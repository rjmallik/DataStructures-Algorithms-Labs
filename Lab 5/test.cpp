void List::insertBefore(ListElement x) {
    // Create a new node for the element x
    Node* newNode = new Node(x);

    // Link the new node with its neighbors
    newNode->next = beforeCursor->next; // Or simply, newNode->next = afterCursor;
    newNode->prev = beforeCursor;

    // Update the existing neighbors to point to the new node
    beforeCursor->next->prev = newNode; // Or simply, afterCursor->prev = newNode;
    beforeCursor->next = newNode;

    // Increment the number of elements in the list
    
   // num_elements++;
    // Adjust cursor only if inserting before the first element (at position 0)
    if (pos_cursor != 0) {
        // Since we're inserting before the first element, afterCursor does not change
        // but we need to update beforeCursor to stay consistent
        beforeCursor = newNode;
        
    } else {
        // Adjust the cursor position to account for the new element
    
        // Here, beforeCursor stays the same since the new element is inserted before the cursor
        // but logically, the cursor moves forward due to the insertion
    }
        num_elements++;
        pos_cursor++;
}
