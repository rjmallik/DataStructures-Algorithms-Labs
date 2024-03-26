/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* GraphTest.c
*A test program for the Graph ADT, containing a main function to test and demonstrate the functionalities of the Graph ADT, ensuring the correct implementation of the graph operations.
***/



#include "Graph.h"
#include <stdio.h>
#define UNDEF -1
#include <stdlib.h>
#include <"List.h">


int main() {
    // Create a new graph with 5 vertices
    Graph G = newGraph(5);\
    Graph T, C;

    // Add edges
    addEdge(G, 1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 2, 4);
    addEdge(G, 2, 5);
    addEdge(G, 3, 5);
    addEdge(G, 4, 5);

    // Print the graph
    printf("Graph G:\n");
    printGraph(stdout, G);
    printf("\n");
    
    // Perform DFS
    printf("Performing DFS on G...\n");
    List S = newList();
    for (int i = 1; i <= getOrder(G); i++) {
        append(S, i);
    }
    DFS(G, S);

    // Print the order after DFS
    printf("Order of vertices after DFS:\n");
    printList(stdout, S);
    printf("\n");

    // Transpose the graph
    printf("Transposing G...\n");
    T = transpose(G);
    printf("Transpose of G:\n");
    printGraph(stdout, T);

    // Perform DFS on the transpose
    printf("Performing DFS on the transpose...\n");
    DFS(T, S);

    // Copy the graph
    printf("Copying G...\n");
    C = copyGraph(G);
    printf("Copy of G:\n");
    printGraph(stdout, C);

    // Perform BFS from vertex 1
    BFS(G, 1);

    // Test getPath, getParent, and getDist
    for (int i = 1; i <= 5; i++) {
        printf("The shortest path from 1 to %d is: ", i);
        List L = newList();
        getPath(L, G, i);
        printList(stdout, L);
        printf("\n");
        printf("The distance from 1 to %d is: %d\n", i, getDist(G, i));
        printf("The parent of %d is: %d\n\n", i, getParent(G, i));
        freeList(&L);
    }

    // Free graph
    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    freeGraph(&C);
    return 0;
}


