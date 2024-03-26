/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* Graph.c
*This source file contains the actual implementation of the Graph ADT defined in Graph.h, including the construction and management of a graph data structure, as well as the implementation of its associated operations like adding edges/arcs, BFS, and others.
***/


#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#define UNDEF -2

// Structs
typedef struct GraphObj {
    List* neighbors; // Adjacency list array
    int* color;      // Color array (white, gray, black)
    int* parent;     // Parent array
    int* distance;   // Distance array
    int order;       // Number of vertices
    int size;        // Number of edges
    int source;      // Most recent source vertex
    int* discover;   // Discover time array
    int* finish;     // Finish time array
} GraphObj;

/*** Constructors-Destructors ***/
Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    G->neighbors = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0;
    G->source = NIL;
    G->discover = calloc(n + 1, sizeof(int));
    G->finish = calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; i++) {
    	G->discover[i] = UNDEF;
    	G->finish[i] = UNDEF;
    }
    for (int i = 1; i <= n; i++) {
        G->neighbors[i] = newList();
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
    }
    return G;
}
void Visit(Graph G, int u, int* time, List S);

Graph transpose(Graph G) {
    Graph T = newGraph(getOrder(G));
    for (int i = 1; i <= getOrder(G); i++) { // Start from 1
        List L = G->neighbors[i];
        for (moveFront(L); index(L) >= 0; moveNext(L)) {
            int v = get(L);
            addArc(T, v, i); // Add arc in reverse direction
        }
    }
    return T;
}


void freeGraph(Graph* pG){
    if(pG != NULL && *pG != NULL){
        for(int i = 0; i <= (*pG)->order; ++i){ // 'order' is the number of vertices
            // Free each adjacency list
            List L = (*pG)->neighbors[i]; // Access the List for vertex 'i'
            freeList(&L); // Assuming 'freeList' properly frees the list and sets its pointer to NULL
        }
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->distance);
        free((*pG)->discover);
        free((*pG)->finish);
        free((*pG)->neighbors);
        (*pG)->neighbors = NULL;
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/
int getOrder(Graph G) {
    return G->order;
}

int getSize(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

int getSource(Graph G) {
    return G->source;
}

int getParent(Graph G, int u) {
    if (u < 1 || u > getOrder(G)) return NIL;
    return G->parent[u];
}

int getDist(Graph G, int u) {
    if (u < 1 || u > getOrder(G)) return INF;
    return G->distance[u];
}

int getDiscover(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getDiscover() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 0 || u > getOrder(G)) { // Assuming getOrder(G) returns the number of vertices
        printf("Graph Error: calling getDiscover() with vertex out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return G->discover[u]; // Assuming discover is an array in GraphObj with discovery times
}

int getFinish(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getFinish() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 0 || u > getOrder(G)) {
        printf("Graph Error: calling getFinish() with vertex out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return G->finish[u]; // Assuming finish is an array in GraphObj with finish times
}

void getPath(List L, Graph G, int u) {
    if (getSource(G) == NIL) {
        fprintf(stderr, "Graph Error: calling getPath() before BFS()\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Graph Error: calling getPath() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    if (u == getSource(G)) {
        append(L, u);
    } else if (G->parent[u] == NIL) {
        append(L, NIL);
    } else {
        getPath(L, G, G->parent[u]);
        append(L, u);
    }
}

/*** Manipulation procedures ***/
void makeNull(Graph G) {
    for (int i = 1; i <= G->order; i++) {
        clear(G->neighbors[i]);
    }
    G->size = 0;
}

void addEdge(Graph G, int u, int v) {
    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;
}

void addArc(Graph G, int u, int v) {
    if (u < 1 || u > G->order || v < 1 || v > G->order) {
        fprintf(stderr, "Graph Error: addArc() called with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    List L = G->neighbors[u];
    // Move to the correct position for insertion or determine the arc exists
    for (moveFront(L); index(L) >= 0; moveNext(L)) {
        if (get(L) > v) { // Found insertion point before current
            insertBefore(L, v);
            G->size++;
            return; // Added new arc, exit function
        } else if (get(L) == v) {
            // Arc already exists, no action needed
            return;
        }
    }
    // If we reach here, we're appending to the end of the list
    append(L, v);
    G->size++;
}
/*
void BFS(Graph G, int s) {
    for (int i = 1; i <= G->order; i++) {
        G->color[i] = WHITE;
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }
    G->source = s;
    G->color[s] = GRAY;
    G->distance[s] = 0;
    G->parent[s] = NIL;

    List Q = newList();
    append(Q, s);
    while (length(Q) > 0) {
        int x = front(Q);
        deleteFront(Q);
        List adj = G->neighbors[x];
        moveFront(adj);
        while (index(adj) >= 0) {
            int y = get(adj);
            if (G->color[y] == WHITE) {
                G->color[y] = GRAY;
                G->distance[y] = G->distance[x] + 1;
                G->parent[y] = x;
                append(Q, y);
            }
            moveNext(adj);
        }
        G->color[x] = BLACK;
    }
    freeList(&Q);
}
*/

void DFS(Graph G, List S) {
    if (length(S) != getOrder(G)) {
        fprintf(stderr, "DFS Error: length of List S does not match the number of vertices in Graph\n");
	exit(EXIT_FAILURE);
    }

    // Initialize vertices
    for (int i = 1; i <= getOrder(G); i++) {
        G->color[i] = WHITE;
        G->parent[i] = NIL;
    }

    int time = 0;
    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        int u = get(S);
        if (G->color[u] == WHITE) {
            Visit(G, u, &time, S);
        }
    }

    // Clear S and store the vertices in order of decreasing finish times
    clear(S);
    for (int i = 1; i <= getOrder(G); i++) {
        for (moveFront(S); index(S) >= 0; moveNext(S)) {
            int v = get(S);
            if (G->finish[v] < G->finish[i]) {
                insertBefore(S, i);
                break;
            }
        }
        if (index(S) == -1) {
            append(S, i);
        }
    }
}

void Visit(Graph G, int u, int* time, List S) {
    G->color[u] = GRAY; // Mark the vertex as discovered but not fully explored
    G->discover[u] = ++(*time); // Set discovery time

    // Explore each neighbor of u
    for (moveFront(G->neighbors[u]); index(G->neighbors[u]) >= 0; moveNext(G->neighbors[u])) {
        int v = get(G->neighbors[u]); // Get the current neighbor
        if (G->color[v] == WHITE) { // If neighbor v is unvisited
            G->parent[v] = u; // Set u as parent of v
            Visit(G, v, time, S); // Recursively visit v
        }
    }

    G->color[u] = BLACK; // Mark the vertex as fully explored
    G->finish[u] = ++(*time); // Set finish time
    prepend(S, u); // Prepend vertex to the list S to track the order of fully explored vertices
}



/*** Other operations ***/
void printGraph(FILE* out, Graph G) {
    for (int i = 1; i <= G->order; i++) {
        fprintf(out, "%d: ", i);
        printList(out, G->neighbors[i]);
        //fprintf(out, "\n");
    }
}

