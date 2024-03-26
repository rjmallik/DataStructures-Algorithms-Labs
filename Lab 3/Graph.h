/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* Graph.h
*This header file defines the structure and interface for the Graph ADT, including the GraphObj type, function prototypes for graph operations (like constructors, destructors, access, manipulation, BFS, and printing functions), and necessary constants.
***/


#ifndef GRAPH_H
#define GRAPH_H

#include "List.h"
#include <stdio.h>

#define INF -1
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define NIL -1
#define UNDEF -2

typedef struct GraphObj* Graph;

// Constructors-Destructors
Graph newGraph(int n);
void freeGraph(Graph* pG);

// Access functions
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
int getDist(Graph G, int u);
int getDiscover(Graph G, int u);
int getFinish(Graph G, int u);
void getPath(List L, Graph G, int u);

// Manipulation procedures
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);
void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */
void Visit(Graph G, int u, int* time, List S);

// Other operations
void printGraph(FILE* out, Graph G);
void insertSorted(List L, int v);
Graph transpose(Graph G);
Graph copyGraph(Graph G);

#endif