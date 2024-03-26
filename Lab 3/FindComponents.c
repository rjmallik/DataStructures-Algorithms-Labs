/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* FindComponents.c
*The main client program which is designed to find and output the strongly connected components (SCCs) of a directed graph.
***/
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
    // Check for command line arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open files
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    if (in == NULL || out == NULL) {
        fprintf(stderr, "Error: Cannot open file %s for reading or writing.\n", (in == NULL) ? argv[1] : argv[2]);
        exit(EXIT_FAILURE);
    }

    // Read the size of the graph
    int size = 0;
    fscanf(in, "%d", &size);
    Graph G = newGraph(size);

    // Read in the edges and construct the graph
    int u, v;
    while (fscanf(in, "%d %d", &u, &v) == 2 && u != 0 && v != 0) {
        addArc(G, u, v);
    }

    // Print the graph
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    //fprintf(out, "\n");
    
    
    
    
    
    // Prepare the list for DFS
    List S = newList();
    for (int i = 1; i <= getOrder(G); ++i) {
        append(S, i);
    }

    // Perform DFS on G and its transpose
    DFS(G, S);
    Graph T = transpose(G);
    DFS(T, S);
    
       
    int SCC_count = 0;
    for(moveBack(S); index(S) >= 0; movePrev(S)) {
        if(getParent(T, get(S)) == NIL) {
            SCC_count++;
        }
    }
    fprintf(out, "\nG contains %d strongly connected components:\n", SCC_count);
    int n= 1;
    List new_list = newList();
    for(moveBack(S); index(S) >= 0; movePrev(S)) {
        int elem = get(S);
        prepend(new_list, elem);
        if(getParent(T, elem) == NIL) {
            fprintf(out, "Component %d: ", n++);
            printList(out, new_list);
            clear(new_list);
        }
    }



    freeGraph(&G);
    freeGraph(&T);
    freeList(&S);
    fclose(in);
    fclose(out);

    return 0;
}