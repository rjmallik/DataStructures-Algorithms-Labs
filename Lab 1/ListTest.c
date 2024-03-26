#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main() {
    // Create a new List and some variables
    List L = newList();
    int i;

    // Append elements in the list
    for(i = 1; i <= 10; i++) {
        append(L, i);
    }

    // Print the list
    printf("After appending 1-10: ");
    printList(stdout, L); // Should print 1 2 3 4 5 6 7 8 9 10
    printf("\n");

    // Move Front and insert before
    moveFront(L);
    insertBefore(L, -1);
    printf("After inserting -1 before first: ");
    printList(stdout, L); // Should print -1 1 2 3 4 5 6 7 8 9 10
    printf("\n");

    // Move Back and insert after
    moveBack(L);
    insertAfter(L, 11);
    printf("After inserting 11 after last: ");
    printList(stdout, L); // Should print -1 1 2 3 4 5 6 7 8 9 10 11
    printf("\n");

    // Test the length function
    printf("List length: %d\n", length(L)); // Should be 12

    // Test front, back, and get functions
    printf("Front element: %d\n", front(L)); // Should be -1
    printf("Back element: %d\n", back(L)); // Should be 11
    moveFront(L);
    printf("Cursor element: %d\n", get(L)); // Should be -1

    // Delete front, back and cursor elements
    deleteFront(L);
    deleteBack(L);
    delete(L);
    printf("After deleting front, back, and cursor elements: ");
    printList(stdout, L); // Should print 1 2 3 4 5 6 7 8 9 10
    printf("\n");

    // Clear the list and test if it's empty
    clear(L);
    printf("After clearing the list: ");
    printList(stdout, L); // Should print nothing
    printf("\nList length after clearing: %d\n", length(L)); // Should be 0

    // Free the list
    freeList(&L);

    return 0;
}
