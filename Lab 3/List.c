/***
*Rutujit(RJ) Mallikarjuna
*rmallika
* 2024 Winter CSE 101 PA3
* List.c
*Contains all List functions necessary to do PA3.
***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "List.h"

// Private Node structure
typedef struct NodeObj {
    int data;
    struct NodeObj* prev;
    struct NodeObj* next;
} NodeObj;

typedef NodeObj* Node;

// Private List structure
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// Constructors-Destructors

List newList(void) {
    List L = malloc(sizeof(ListObj));
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}

void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}

// Access functions

int length(List L) {
    return L->length;
}

int index(List L) {
    return L->index;
}

int front(List L) {
    if (length(L) > 0) {
        return L->front->data;
    } else {
        fprintf(stderr, "Error: front() called on empty list.\n");
        exit(EXIT_FAILURE);
    }
}

int back(List L) {
    if (length(L) > 0) {
        return L->back->data;
    } else {
        fprintf(stderr, "Error: back() called on empty list.\n");
        exit(EXIT_FAILURE);
    }
}

int get(List L) {
    if (length(L) > 0 && index(L) >= 0) {
        return L->cursor->data;
    } else {
        fprintf(stderr, "Error: get() called on empty list or undefined cursor.\n");
        exit(EXIT_FAILURE);
    }
}

bool equals(List A, List B) {
    if (length(A) != length(B)) {
        return false;
    }

    Node nodeA = A->front;
    Node nodeB = B->front;

    while (nodeA != NULL) {
        if (nodeA->data != nodeB->data) {
            return false;
        }
        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }

    return true;
}

// Manipulation procedures

void clear(List L) {
    while (length(L) > 0) {
        deleteFront(L);
    }
}

void set(List L, int x) {
    if (length(L) > 0 && index(L) >= 0) {
        L->cursor->data = x;
    } else {
        fprintf(stderr, "Error: set() called on empty list or undefined cursor.\n");
        exit(EXIT_FAILURE);
    }
}

void moveFront(List L) {
    if (length(L) > 0) {
        L->cursor = L->front;
        L->index = 0;
    }
}

void moveBack(List L) {
    if (length(L) > 0) {
    	L->cursor = NULL;
        L->cursor = L->back;
        L->index = length(L) - 1;
    }
}

void movePrev(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // If the cursor is undefined or at the front, do nothing or make it undefined
    if (L->cursor == NULL || L->cursor == L->front) {
        L->cursor = NULL;
        L->index = -1;
    } else {
        // Move cursor one step toward the front
        L->cursor = L->cursor->prev;
        L->index--;
    }
}


void moveNext(List L) {
    if (index(L) >= 0) {
        if (L->cursor != L->back) {
            L->cursor = L->cursor->next;
            L->index++;
        } else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}
void prepend(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node newNode = malloc(sizeof(NodeObj));
    if (newNode == NULL) {
        fprintf(stderr, "List Error: malloc failed in prepend()\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = x;
    newNode->prev = NULL;
    newNode->next = L->front;

    if (L->front == NULL) {  // List is empty
        L->front = L->back = newNode;
    } else {  // List is not empty
        L->front->prev = newNode;
        L->front = newNode;
    }

    L->length++;

    // Update cursor index if it is defined
    if (L->index >= 0) {
        L->index++;
    }
}

void append(List L, int x) {
    Node newNode = malloc(sizeof(NodeObj));
    newNode->data = x;
    newNode->prev = L->back;
    newNode->next = NULL;

    if (length(L) == 0) {
        L->front = newNode;
    } else {
        L->back->next = newNode;
    }

    L->back = newNode;
    L->length++;
}

void insertBefore(List L, int x) {
    if (length(L) > 0 && index(L) >= 0) {
        Node newNode = malloc(sizeof(NodeObj));
        newNode->data = x;
        newNode->prev = L->cursor->prev;
        newNode->next = L->cursor;

        if (L->cursor->prev != NULL) {
            L->cursor->prev->next = newNode;
        } else {
            L->front = newNode;
        }

        L->cursor->prev = newNode;
        L->length++;
        L->index++;
    } else {
        fprintf(stderr, "Error: insertBefore() called on empty list or undefined cursor.\n");
        exit(EXIT_FAILURE);
    }
}

void insertAfter(List L, int x) {
    if (length(L) > 0 && index(L) >= 0) {
        Node newNode = malloc(sizeof(NodeObj));
        newNode->data = x;
        newNode->prev = L->cursor;
        newNode->next = L->cursor->next;

        if (L->cursor->next != NULL) {
            L->cursor->next->prev = newNode;
        } else {
            L->back = newNode;
        }

        L->cursor->next = newNode;
        L->length++;
    } else {
        fprintf(stderr, "Error: insertAfter() called on empty list or undefined cursor.\n");
        exit(EXIT_FAILURE);
    }
}

void deleteFront(List L) {
    if (length(L) > 0) {
        Node oldFront = L->front;

        if (length(L) == 1) {
            L->front = L->back = L->cursor = NULL;
            L->index = -1;
        } else {
            L->front = L->front->next;
            L->front->prev = NULL;

            if (L->cursor != NULL) {
                L->index--;
            }
        }

        free(oldFront);
        L->length--;
    } else {
        fprintf(stderr, "Error: deleteFront() called on empty list.\n");
        exit(EXIT_FAILURE);
    }
}

void deleteBack(List L) {
    if (length(L) > 0) {
        Node oldBack = L->back;

        if (length(L) == 1) {
            L->front = L->back = L->cursor = NULL;
            L->index = -1;
        } else {
            L->back = L->back->prev;
            L->back->next = NULL;

            if (L->cursor == oldBack) {
                L->cursor = NULL;
                L->index = -1;
            }
        }

        free(oldBack);
        L->length--;
    } else {
        fprintf(stderr, "Error: deleteBack() called on empty list.\n");
        exit(EXIT_FAILURE);
    }
}

void delete(List L) {
    if (length(L) > 0 && index(L) >= 0) {
        Node cursorNode = L->cursor;

        if (L->cursor->prev != NULL) {
            L->cursor->prev->next = L->cursor->next;
        } else {
            L->front = L->cursor->next;
        }

        if (L->cursor->next != NULL) {
            L->cursor->next->prev = L->cursor->prev;
        } else {
            L->back = L->cursor->prev;
        }

        free(cursorNode);
        L->cursor = NULL;
        L->index = -1;
        L->length--;
    } else {
        fprintf(stderr, "Error: delete() called on empty list or undefined cursor.\n");
        exit(EXIT_FAILURE);
    }
}

// Other operations

void printList(FILE* out, List L) {
    Node current = L->front;

    while (current != NULL) {
        fprintf(out, "%d ", current->data);
        current = current->next;
    }

    fprintf(out, "\n");
}

List copyList(List L) {
    List copy = newList();
    Node current = L->front;

    while (current != NULL) {
        append(copy, current->data);
        current = current->next;
    }

    return copy;
}

List concatList(List A, List B) {
    List concat = copyList(A);
    Node currentB = B->front;

    while (currentB != NULL) {
        append(concat, currentB->data);
        currentB = currentB->next;
    }

    return concat;
}
