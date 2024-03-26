/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA@
* Graph.h
*This header file defines the structure and interface for the Graph ADT, including the GraphObj type, function prototypes for graph operations (like constructors, destructors, access, manipulation, BFS, and printing functions), and necessary constants.
***/


#ifndef GRAPH_H
#define GRAPH_H

#include "List.h"
#include <stdio.h>

#define INF -1
#define NIL 0
#define WHITE 1
#define GRAY 2
#define BLACK 3

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
void getPath(List L, Graph G, int u);

// Manipulation procedures
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);

// Other operations
void printGraph(FILE* out, Graph G);
void insertSorted(List L, int v);

#endif
