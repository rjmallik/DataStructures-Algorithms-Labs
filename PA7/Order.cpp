
#include "Dictionary.h"

#include <fstream>
#include <stdio.h>

int main(int argc, char **argv) {
    std::fstream infile, outfile;
    std::string n;
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return -1;
    }
    infile.open(argv[1]);
    outfile.open(argv[2], std::fstream::out);

    // Keep adding keys in alphabetical order
    Dictionary D;
    int i = 1;
    while (infile >> n) {
        D.setValue(n, i);
        i++;
    }
    infile.close();

    // Keys only, one to a line
    outfile << D << std::endl;
    std::string s = D.pre_string();
    if (!s.empty()) {
        s.pop_back();
    }
    outfile << s << std::endl;
    outfile.close();
}
