#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main() {
    int n = 5; // Example size
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    // Populate matrices A and B
    changeEntry(A, 1, 1, 1.0); changeEntry(A, 1, 2, 2.0);
    changeEntry(B, 1, 1, 1.0); changeEntry(B, 3, 3, 5.0);

    // Print A and B
    printf("Matrix A:\n");
    printMatrix(stdout, A);
    printf("\nMatrix B:\n");
    printMatrix(stdout, B);

    // Test arithmetic operations
    Matrix sumAB = sum(A, B);
    printf("\nSum of A and B:\n");
    printMatrix(stdout, sumAB);

    Matrix diffAB = diff(A, B);
    printf("\nDifference of A and B:\n");
    printMatrix(stdout, diffAB);

    Matrix productAB = product(A, B);
    printf("\nProduct of A and B:\n");
    printMatrix(stdout, productAB);

    // Test scalar multiplication, transpose, and copy
    Matrix scalarA = scalarMult(2.0, A);
    printf("\nScalar multiplication of A by 2:\n");
    printMatrix(stdout, scalarA);

    Matrix transA = transpose(A);
    printf("\nTranspose of A:\n");
    printMatrix(stdout, transA);

    Matrix copyA = copy(A);
    printf("\nCopy of A:\n");
    printMatrix(stdout, copyA);

    // Clean up
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&sumAB);
    freeMatrix(&diffAB);
    freeMatrix(&productAB);
    freeMatrix(&scalarA);
    freeMatrix(&transA);
    freeMatrix(&copyA);

    printf("\nMatrix tests completed.\n");

    return 0;
}
