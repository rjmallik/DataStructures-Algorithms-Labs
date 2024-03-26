/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA@
* FindPath.c
*A main program that utilizes the Graph ADT to find paths in a graph, typically reading input to construct a graph, perform BFS, and then output the shortest paths to a file or standard output.
***/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

#define MAX_LEN 100

int main(int argc, char* argv[]) {
    // Check for correct number of command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // Open files
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (in == NULL || out == NULL) {
        fprintf(stderr, "Error: Unable to open file \"%s\" for reading or \"%s\" for writing.\n", argv[1], argv[2]);
        exit(1);
    }

    // Read the number of vertices
    int numVertices;
    fscanf(in, "%d", &numVertices);
    Graph G = newGraph(numVertices);

    // Read the edges and construct the graph
    int u, v;
    while (fscanf(in, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break; // End of graph definition
        addEdge(G, u, v);
    }

    // Print the graph's adjacency list representation
    printGraph(out, G);

    // Process the pairs of vertices and find shortest paths
    while (fscanf(in, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break; // End of input

        BFS(G, u);
        List L = newList();
        getPath(L, G, v);

        // Print the distance and path
        fprintf(out, "\nThe distance from %d to %d is ", u, v);
        if (getDist(G, v) == INF) {
            fprintf(out, "infinity\n");
            fprintf(out, "No %d-%d path exists\n", u, v);
        } else {
            fprintf(out, "%d\n", getDist(G, v));
            fprintf(out, "A shortest %d-%d path is: ", u, v);
            printList(out, L);
            fprintf(out, "\n");
        }

        freeList(&L);
    }

    // Free resources
    freeGraph(&G);
    fclose(in);
    fclose(out);

    return 0;
}

