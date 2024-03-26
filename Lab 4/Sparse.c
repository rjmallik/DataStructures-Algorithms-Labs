#include <stdio.h>
#include <stdlib.h>
#include "List.h"
// Assuming Matrix.h provides the Matrix ADT and related operations
#include "Matrix.h"

int main(int argc, char* argv[]) {
    FILE *in, *out;
    int n, a, b;

    // Check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    // Open files for reading and writing
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        return 1;
    }
    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        return 1;
    }

    // Read in n, a, b
    fscanf(in, "%d %d %d", &n, &a, &b);
    // Consume the newline character after the first line
    fgetc(in);

    // Initialize matrices A and B
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    // Read matrix A
    for (int i = 0; i < a; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(A, row, col, value);
    }

    // Skip the blank line between A and B
    fgetc(in);
    fgetc(in);

    // Read matrix B
    for (int i = 0; i < b; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(B, row, col, value);
    }

    // Perform operations and print results
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);

    // Calculate (1.5)*A and print
    Matrix scalarMultA = scalarMult(1.5, A);
    fprintf(out, "\n(1.5)*A =\n");
    printMatrix(out, scalarMultA);

    // A+B
    Matrix sumAB = sum(A, B);
    fprintf(out, "\nA+B =\n");
    printMatrix(out, sumAB);

    // A+A
    Matrix sumAA = sum(A, A);
    fprintf(out, "\nA+A =\n");
    printMatrix(out, sumAA);

    // B-A
    Matrix diffBA = diff(B, A);
    fprintf(out, "\nB-A =\n");
    printMatrix(out, diffBA);

    // A-A
    Matrix diffAA = diff(A, A);
    fprintf(out, "\nA-A =\n");
    printMatrix(out, diffAA);

    // Transpose(A)
    Matrix transposeA = transpose(A);
    fprintf(out, "\nTranspose(A) =\n");
    printMatrix(out, transposeA);

    // A*B
    Matrix productAB = product(A, B);
    fprintf(out, "\nA*B =\n");
    printMatrix(out, productAB);

    // B*B
    Matrix productBB = product(B, B);
    fprintf(out, "\nB*B =\n");
    printMatrix(out, productBB);

    // Free all matrices
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&scalarMultA);
    freeMatrix(&sumAB);
    freeMatrix(&sumAA);
    freeMatrix(&diffBA);
    freeMatrix(&diffAA);
    freeMatrix(&transposeA);
    freeMatrix(&productAB);
    freeMatrix(&productBB);

    // Close files
    fclose(in);
    fclose(out);

    return 0;
}
