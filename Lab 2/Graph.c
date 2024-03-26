/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA@
* Graph.c
*This source file contains the actual implementation of the Graph ADT defined in Graph.h, including the construction and management of a graph data structure, as well as the implementation of its associated operations like adding edges/arcs, BFS, and others.
***/


#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// Structs
typedef struct GraphObj {
    List* neighbors; // Adjacency list array
    int* color;      // Color array (white, gray, black)
    int* parent;     // Parent array
    int* distance;   // Distance array
    int order;       // Number of vertices
    int size;        // Number of edges
    int source;      // Most recent source vertex
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

    for (int i = 1; i <= n; i++) {
        G->neighbors[i] = newList();
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
    }
    return G;
}

void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) {
        for (int i = 1; i <= (*pG)->order; i++) {
            freeList(&((*pG)->neighbors[i]));
        }
        free((*pG)->neighbors);
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->distance);
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/
int getOrder(Graph G) {
    return G->order;
}

int getSize(Graph G) {
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
        fprintf(stderr, "Graph Error: calling addArc() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    List L = G->neighbors[u];
    moveFront(L);
    while (index(L) >= 0 && get(L) < v) {
        moveNext(L);
    }
    if (index(L) == -1) {
        append(L, v);
    } else {
        insertBefore(L, v);
    }
    G->size++;
}

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

/*** Other operations ***/
void printGraph(FILE* out, Graph G) {
    for (int i = 1; i <= G->order; i++) {
        fprintf(out, "%d: ", i);
        printList(out, G->neighbors[i]);
        fprintf(out, "\n");
    }
}

