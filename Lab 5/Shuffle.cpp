#include <iostream>
#include <cstdlib> // For atoi()
#include "List.h"

// Function to perform one shuffle operation as described
void shuffle(List& D) {
    int length = D.length();
    List leftHalf, rightHalf;
    
    // Split the deck into two halves
    D.moveFront();
    for(int i = 0; i < length/2; ++i) {
        leftHalf.insertBefore(D.moveNext());
    }
    for(int i = length/2; i < length; ++i) {
        rightHalf.insertBefore(D.moveNext());
    }

    // Prepare to merge by clearing D and setting up iterators for left and right halves
    D.clear();
    leftHalf.moveFront();
    rightHalf.moveFront();
    
    // Merge the two halves back into D
    while (leftHalf.position() < leftHalf.length() || rightHalf.position() < rightHalf.length()) {
        if(rightHalf.position() < rightHalf.length()) {
            D.insertBefore(rightHalf.moveNext());
        }
        if(leftHalf.position() < leftHalf.length()) {
            D.insertBefore(leftHalf.moveNext());
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <max deck size>" << std::endl;
        return EXIT_FAILURE;
    }

    int maxDeckSize = atoi(argv[1]);
    std::cout << "deck size shuffle count" << std::endl;
    std::cout << "------------------------------" << std::endl;

    for(int n = 1; n <= maxDeckSize; ++n) {
        List D;
        for(int i = 0; i < n; ++i) {
            D.insertBefore(i);
        }

        int shuffleCount = 0;
        List original(D);
        do {
            shuffle(D);
            ++shuffleCount;
        } while (!D.equals(original));

        std::cout << n << " " << shuffleCount << std::endl;
    }

    return EXIT_SUCCESS;
}
