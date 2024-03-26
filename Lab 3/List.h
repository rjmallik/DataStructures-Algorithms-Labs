/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* List.h
*Contains all List ADT operations used for the PA3.
***/

#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_

// Exported type
typedef struct ListObj* List;

// Constructors-Destructors
List newList(void); 
void freeList(List* pL);

// Access functions
int length(List L);
int index(List L);
int front(List L);
int back(List L);
int get(List L);

// Manipulation procedures
void clear(List L);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, int data);
void append(List L, int data);
void insertBefore(List L, int data);
void insertAfter(List L, int data);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);

// Other operations
void printList(FILE* out, List L);
List copyList(List L);

#endif
