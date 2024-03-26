/***
*Rutujit(RJ) Mallikarjuna
*rmallika
*2024 Winter CSE 101 PA6
*Arithmetic.cpp
* Top level client file for this project.
***/
#include "BigInteger.h"
#include <fstream>
#include <iostream>
#include <string>
#include <limits>


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files.\n";
        return 1;
    }

    std::string lineA, lineB;
    std::getline(inputFile, lineA);
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip blank line
    std::getline(inputFile, lineB);

    BigInteger A(lineA), B(lineB);

    // Perform arithmetic operations
    BigInteger sum = A + B;
    BigInteger diff = A - B;
    BigInteger sameDiff = A - A;
    BigInteger customDiff = 3 * A - 2 * B;
    BigInteger product = A * B;
    BigInteger ASquared = A * A;
    BigInteger BSquared = B * B;
    BigInteger complexOperation = 9 * (ASquared * ASquared) + 16 * (BSquared * BSquared * B);

    // Write results to output file
    outputFile << A << "\n\n" << B << "\n\n" << sum << "\n\n" << diff << "\n\n";
    outputFile << sameDiff << "\n\n" << customDiff << "\n\n" << product << "\n\n";
    outputFile << ASquared << "\n\n" << BSquared << "\n\n" << complexOperation << "\n";

    inputFile.close();
    outputFile.close();

    return 0;
}
