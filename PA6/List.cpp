/***
*Rutujit(RJ) Mallikarjuna
*rmallika
*2024 Winter CSE 101 PA3
*List.cpp
* Main client function to run the program and to run for all the test cases for the programming lab.
***/
#include "List.h"
#include <iostream>
#include <stdexcept> // For standard exception objects

// Node constructor
List::Node::Node(ListElement x) : data(x), next(nullptr), prev(nullptr) {}

// List constructor
List::List() {
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

// Copy constructor
List::List(const List& L) : List() {
    *this = L; // Use assignment operator
}

// Destructor
List::~List() {
    clear();
    delete frontDummy;
    delete backDummy;
}

// Access functions
int List::length() const {
    return num_elements;
}

ListElement List::front() const {
    if (length() <= 0) throw std::logic_error("List is empty");
    return frontDummy->next->data;
}

ListElement List::back() const {
    if (length() <= 0) throw std::logic_error("List is empty");
    return backDummy->prev->data;
}

int List::position() const {
    return pos_cursor;
}

ListElement List::peekNext() const {
    if (position() >= length()) throw std::logic_error("No next element");
    return afterCursor->data;
}

ListElement List::peekPrev() const {
    if (position() <= 0) throw std::logic_error("No previous element");
    return beforeCursor->data;
}

// Manipulation procedures
void List::clear() {
    moveFront();
    while (frontDummy->next != backDummy) {
        eraseAfter();
    }
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

void List::moveFront() {
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

void List::moveBack() {
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    pos_cursor = num_elements;
}

ListElement List::moveNext() {
    if (position() >= length()) { // Checks if the cursor is already at the end.
        throw std::logic_error("Cursor at the end");
    }
    // Move the cursor forward only if 'afterCursor' is not pointing to 'backDummy'.
    if (afterCursor->next != nullptr) {
        beforeCursor = afterCursor;
        afterCursor = afterCursor->next;
        pos_cursor++;
        return beforeCursor->data; // Return the data that the cursor just passed.
    } else {
        // Handle the scenario where 'afterCursor' is pointing to 'backDummy',
        // which might require special handling or to simply throw an exception.
        throw std::logic_error("No next element to move to");
    }
}

ListElement List::movePrev() {
    if (position() <= 0) throw std::logic_error("Cursor at the start");
    ListElement data = beforeCursor->data;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    pos_cursor--;
    return data;
}

void List::insertAfter(ListElement x) {
    Node* newNode = new Node(x);
    newNode->next = afterCursor;
    newNode->prev = beforeCursor;
    beforeCursor->next = newNode;
    afterCursor->prev = newNode;
    afterCursor = newNode;
    num_elements++;
}

void List::insertBefore(ListElement x) {
    Node* newNode = new Node(x);
    newNode->next = afterCursor;
    newNode->prev = beforeCursor;
    beforeCursor->next = newNode;
    afterCursor->prev = newNode;
    beforeCursor = newNode;
    if (pos_cursor == 0) afterCursor = newNode->next; // Special case for insert at front
    num_elements++;
    pos_cursor++;
}

void List::eraseAfter() {
    if (position() >= length() || afterCursor == backDummy) { // Combined check for edge conditions
        throw std::logic_error("No element to erase after cursor or at the end");
    }
    Node* toDelete = afterCursor;
    beforeCursor->next = toDelete->next; // Link beforeCursor to the node after toDelete
    toDelete->next->prev = beforeCursor; // Link the node after toDelete back to beforeCursor
    if (toDelete->next == backDummy) { // If deleting the last real node
        afterCursor = backDummy; // Ensure afterCursor points to the dummy tail
    } else {
        afterCursor = toDelete->next; // Normal case
    }
    delete toDelete;
    num_elements--;
}


void List::eraseBefore() {
    if (position() <= 0) throw std::logic_error("No element to erase before cursor");
    Node* toDelete = beforeCursor;
    afterCursor->prev = beforeCursor->prev;
    beforeCursor->prev->next = afterCursor;
    beforeCursor = beforeCursor->prev;
    delete toDelete;
    num_elements--;
    pos_cursor--;
}
// Assumes ListElement is a type for which comparison operators are defined
int List::findNext(ListElement x) {
    Node* temp = afterCursor;
    int tempIndex = pos_cursor + 1;

    while (temp != backDummy) {
        if (temp->data == x) {
            // Found the element, update cursor and return position
            beforeCursor = temp; // Update the beforeCursor to the found node
            afterCursor = temp->next; // Update the afterCursor to the next node
            pos_cursor = tempIndex; // Update the cursor position
            return pos_cursor; // Return the updated position
        }
        temp = temp->next; // Move to the next node
        tempIndex++; // Increment the index
    }

    // Element not found, move cursor to the back
    moveBack();
    return -1; // Return -1 to indicate failure to find the element
}


int List::findPrev(ListElement x) {
    if (num_elements == 0) {
        // If the list is empty, move the cursor to the front and return -1
        moveFront();
        return -1;
    }

    int tempPos = pos_cursor - 1; // Start from the position before the cursor
    Node* temp = beforeCursor; // Start from the node before the cursor

    // Use a while loop to iterate through the list backwards
    while (temp != frontDummy) {
        if (temp->data == x) {
            // If the current node's data matches the search value
            pos_cursor = tempPos; // Update the cursor position
            beforeCursor = temp->prev; // Update beforeCursor to the previous node
            afterCursor = temp; // Update afterCursor to the current node
            return pos_cursor; // Return the new cursor position
        }
        temp = temp->prev; // Move to the previous node
        tempPos--; // Decrement the position counter
    }

    // If the value is not found, move the cursor to the front
    moveFront();
    return -1; // Return -1 to indicate that the value was not found
}


//cleanup()
void List::cleanup() {
    if (num_elements <= 1) {
	return;
	}

    Node* curr = frontDummy->next;
    int currIndex = 0;
    while (curr != backDummy && curr->next != backDummy) {
        Node* temp = curr->next;
	int tempIndex = currIndex;
        while (temp != backDummy) {//Thanks to TA helped my while loop
            if (curr->data == temp->data) {
                Node* n = temp;//n is duplicate
                temp = temp->next;
                n->prev->next = n->next;
                n->next->prev = n->prev;
                delete n;
                num_elements--;
		if (tempIndex < pos_cursor) {
			pos_cursor--;
		}
            } else {
                temp = temp->next;
            }
	    tempIndex++;
        }
        curr = curr->next;
	currIndex++;
    }

    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    for (int curr_Pos = 0; curr_Pos < pos_cursor && afterCursor != backDummy; curr_Pos++) {
        beforeCursor = afterCursor;
        afterCursor = afterCursor->next;
    }
}


List List::concat(const List& L) const {
    List newList;
    Node* node = this->frontDummy->next;
    while (node != this->backDummy) {
        newList.insertBefore(node->data); // Insert elements of the current list
        node = node->next;
    }
    node = L.frontDummy->next;
    while (node != L.backDummy) {
        newList.insertBefore(node->data); // Insert elements of L
        node = node->next;
    }
    newList.moveFront();
    return newList;
}


std::string List::to_string() const {
    std::string str = "";
    Node* current = frontDummy->next;
    while (current != backDummy) {
        str += std::to_string(current->data) + " ";
        current = current->next;
    }
    return str;
}

bool List::isEmpty() const {
    return this->length() == 0; // Assuming there's a length() method that returns the number of elements in the list
}


bool List::equals(const List& R) const {
    bool equal = (num_elements == R.num_elements);
    Node* currentLeft = frontDummy->next;
    Node* currentRight = R.frontDummy->next;
    while (equal && currentLeft != backDummy) {
        if (currentLeft->data != currentRight->data) {
            equal = false;
        }
        currentLeft = currentLeft->next;
        currentRight = currentRight->next;
    }
    return equal;
}
std::ostream& operator<<(std::ostream& out, const List& list) {
    List temp = list; // Copy to use non-const methods if necessary
    temp.moveFront();
    for (int i = 0; i < temp.length(); ++i) {
        out << temp.peekNext();
        if (i < temp.length() - 1) out << " ";
        temp.moveNext();
    }
    return out;
}

bool operator==(const List& A, const List& B) {
    if (A.length() != B.length()) return false;
    List::Node* nodeA = A.frontDummy->next;
    List::Node* nodeB = B.frontDummy->next;
    while (nodeA != A.backDummy && nodeB != B.backDummy) {
        if (nodeA->data != nodeB->data) return false;
        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }
    return true;
}

List& List::operator=(const List& L) {
    if (this != &L) { // Avoid self-assignment
        clear(); // Clear current list
        List::Node* node = L.frontDummy->next;
        while (node != L.backDummy) {
            insertBefore(node->data); // Insert at the end
            //moveNext(); // Move cursor to simulate insertion at the end
            node = node->next;
        }
    }
    return *this;
}
