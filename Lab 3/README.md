Graph.h: This header file defines the structure and interface for the Graph ADT, including the GraphObj type, function prototypes for graph operations (like constructors, destructors, access, manipulation, BFS, and printing functions), and necessary constants.

Graph.c: This source file contains the actual implementation of the Graph ADT defined in Graph.h, including the construction and management of a graph data structure, as well as the implementation of its associated operations like adding edges/arcs, BFS, and others.

List.c: Implements the List ADT, providing functionality for creating and manipulating a list data structure, including operations like insertion, deletion, and traversal, which are used in the implementation of the Graph ADT.

List.h: The header file for the List ADT, defining the structure of a list and the prototypes for list operations, serving as the interface for list functionality used in other parts of the program, particularly in Graph.c.

GraphTest.c: A test program for the Graph ADT, containing a main function to test and demonstrate the functionalities of the Graph ADT, ensuring the correct implementation of the graph operations.

FindPath.c: A main program that utilizes the Graph ADT to find paths in a graph, typically reading input to construct a graph, perform BFS, and then output the shortest paths to a file or standard output.