#include <iostream>
#include "List.h"

void testList() {
    // Test default constructor
    List lst;
    std::cout << "After default construction: " << lst << std::endl;

    // Test insertions
    lst.insertBefore(1);
    lst.insertBefore(2);
    std::cout << "After insertions: " << lst << std::endl;

    // Test move operations
    lst.moveFront();
    lst.moveNext();
    std::cout << "After moving cursor: " << lst << std::endl;

    // Test copy constructor
    List lst2 = lst;
    std::cout << "Copy constructed list: " << lst2 << std::endl;

    // Test assignment operator
    List lst3;
    lst3 = lst;
    std::cout << "Assigned list: " << lst3 << std::endl;

    // Test equality
    std::cout << "List 1 and List 2 are " << (lst == lst2 ? "equal" : "not equal") << std::endl;

    // Test erase operations
    lst.eraseBefore();
    std::cout << "After erasing: " << lst << std::endl;

    // Test clear
    lst.clear();
    std::cout << "After clearing: " << lst << std::endl;
}

int main() {
    try {
        testList();
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    return 0;
}
