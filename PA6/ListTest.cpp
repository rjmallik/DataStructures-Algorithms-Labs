#include "List.h"
#include <cassert>
#include <iostream>
#include <sstream>

void testListCreationAndDestruction() {
    List myList;
    assert(myList.length() == 0);
    std::cout << "Test List Creation and Destruction Passed\n";
}

void testInsertionAndFrontBack() {
    List myList;
    myList.insertBefore(1);
    myList.insertBefore(2);
    assert(myList.front() == 2);
    assert(myList.back() == 1);
    std::cout << "Test Insertion and Front/Back Passed\n";
}

void testDeletion() {
    List myList;
    myList.insertBefore(1);
    myList.insertBefore(2);
    myList.moveFront();
    myList.eraseAfter(); // Erase the 1
    assert(myList.length() == 1 && myList.back() == 2);
    std::cout << "Test Deletion Passed\n";
}

void testSearch() {
    List myList;
    myList.insertBefore(5);
    myList.insertBefore(10);
    myList.moveFront();
    assert(myList.findNext(5) != -1);
    std::cout << "Test Search Passed\n";
}

void testConcat() {
    List list1, list2;
    list1.insertBefore(1);
    list2.insertBefore(2);
    List list3 = list1.concat(list2);
    assert(list3.length() == 2);
    std::cout << "Test Concat Passed\n";
}

void testCleanup() {
    List myList;
    myList.insertBefore(1);
    myList.insertBefore(1);
    myList.cleanup();
    assert(myList.length() == 1);
    std::cout << "Test Cleanup Passed\n";
}

void testEquality() {
    List list1, list2;
    list1.insertBefore(1);
    list2.insertBefore(1);
    assert(list1 == list2);
    std::cout << "Test Equality Passed\n";
}

void testOutput() {
    List myList;
    myList.insertBefore(1);
    std::stringstream ss;
    ss << myList;
    assert(ss.str() == "1");
    std::cout << "Test Output Passed\n";
}

int main() {
    testListCreationAndDestruction();
    testInsertionAndFrontBack();
    testDeletion();
    testSearch();
    testConcat();
    testCleanup();
    testEquality();
    testOutput();

    std::cout << "All tests passed successfully.\n";
    return 0;
}
