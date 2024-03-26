#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include <limits.h>
#include <stdbool.h>

typedef struct EntryObj {
    int col;
    double value;
} EntryObj;

typedef EntryObj* Entry;

typedef struct MatrixObj {
    List* rows;
    int size;
    int nnz;
} MatrixObj;

static Entry newEntry(int col, double value) {
    Entry e = malloc(sizeof(EntryObj));
    if (e == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    e->col = col;
    e->value = value;
    return e;
}

static void freeEntry(Entry* e) {
    if (e != NULL && *e != NULL) {
        free(*e);
        *e = NULL;
    }
}

static List subtractLists(List A, List B) {
    List result = newList();
    moveFront(A);
    moveFront(B);
    while (index(A) >= 0 || index(B) >= 0) {
        Entry a = (index(A) >= 0) ? (Entry)get(A) : NULL;
        Entry b = (index(B) >= 0) ? (Entry)get(B) : NULL;

        int aCol = a ? a->col : INT_MAX;
        int bCol = b ? b->col : INT_MAX;

        if (a && (!b || aCol < bCol)) {
            append(result, newEntry(a->col, a->value));
            moveNext(A);
        } else if (b && (!a || bCol < aCol)) {
            append(result, newEntry(b->col, -b->value));
            moveNext(B);
        } else if (a && b && aCol == bCol) {
            double subtractedValue = a->value - b->value;
            if (subtractedValue != 0) {
                append(result, newEntry(a->col, subtractedValue));
            }
            moveNext(A);
            moveNext(B);
        }
    }
    return result;
}


static List addLists(List A, List B) {
    List sumList = newList();
    moveFront(A);
    moveFront(B);
    while (index(A) >= 0 || index(B) >= 0) {
        Entry a = index(A) >= 0 ? (Entry)get(A) : NULL;
        Entry b = index(B) >= 0 ? (Entry)get(B) : NULL;
        int colA = a ? a->col : INT_MAX;
        int colB = b ? b->col : INT_MAX;

        if (colA < colB) {
            append(sumList, newEntry(a->col, a->value));
            moveNext(A);
        } else if (colA > colB) {
            append(sumList, newEntry(b->col, b->value));
            moveNext(B);
        } else if (colA == colB && colA != INT_MAX) {
            double sumValue = a->value + b->value;
            if (sumValue != 0) {
                append(sumList, newEntry(a->col, sumValue));
            }
            if (A != B){
            	moveNext(A);
            	}
            moveNext(B);
        }
    }
    return sumList;
}
Matrix newMatrix(int n) {
    Matrix M = malloc(sizeof(MatrixObj));
    if (M == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    M->rows = malloc((n+1) * sizeof(List));
    if (M->rows == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(M);
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i <= n; i++) {
        M->rows[i] = newList();
    }
    M->size = n;
    M->nnz = 0;
    return M;
}

void freeMatrix(Matrix* pM) {
    if(pM != NULL && *pM != NULL) {
        Matrix M = *pM;
        for(int i = 1; i <= M->size; i++) {
            List L = M->rows[i];
            moveFront(L);
            while(index(L) >= 0) {
                Entry E = (Entry)get(L);
                freeEntry(&E);
                moveNext(L);
            }
            freeList(&L);
        }
        free(M->rows);
        free(M);
        *pM = NULL;
    }
}

int size(Matrix M) {
    return M->size;
}

int NNZ(Matrix M) {
    return M->nnz;
}

int equals(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        printf("Matrix Error: calling equals() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    if (A->size != B->size) return 0;

    for (int i = 1; i <= A->size; i++) {
        List rowA = A->rows[i];
        List rowB = B->rows[i];
        
        moveFront(rowA);
        moveFront(rowB);

        while (index(rowA) >= 0 && index(rowB) >= 0) {
            Entry eA = (Entry)get(rowA);
            Entry eB = (Entry)get(rowB);
            
            if (eA->col != eB->col) return 0;
            if (eA->value != eB->value) return 0;

            moveNext(rowA);
            moveNext(rowB);
        }

        if (index(rowA) >= 0 || index(rowB) >= 0) return 0;
    }

    return 1;
}

void makeZero(Matrix M) {
    for (int i = 1; i <= M->size; i++) {
        clear(M->rows[i]);
    }
    M->nnz = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
    List row = M->rows[i];
    moveFront(row);
    Entry E = NULL;
    bool found = false;

    while (index(row) >= 0) {
        E = (Entry)get(row);
        if (E->col == j) {
            found = true;
            if (x == 0) {
                freeEntry(&E); // Free the entry if its value is set to 0
                delete(row); // Remove the entry from the list
                M->nnz--;
            } else {
                E->value = x; // Update the entry's value if x is not 0
            }
            break;
        } else if (E->col > j) {
            break;
        }
        moveNext(row);
    }

    if (!found && x != 0) {
        E = newEntry(j, x); // Create a new entry if no matching column is found
        if (index(row) == -1) {
            append(row, E); // Append the new entry if we're at the end of the list
        } else {
            insertBefore(row, E); // Insert the new entry before the current if a larger column is found
        }
        M->nnz++;
    }
}


Matrix copy(Matrix A) {
    Matrix newM = newMatrix(A->size);
    for (int i = 1; i <= A->size; i++) {
        List row = A->rows[i];
        moveFront(row);
        while (index(row) >= 0) {
            Entry E = (Entry)get(row);
            changeEntry(newM, i, E->col, E->value);
            moveNext(row);
        }
    }
    return newM;
}

Matrix transpose(Matrix A) {
    Matrix newM = newMatrix(A->size);
    for (int i = 1; i <= A->size; i++) {
        List row = A->rows[i];
        moveFront(row);
        while (index(row) >= 0) {
            Entry E = (Entry)get(row);
            changeEntry(newM, E->col, i, E->value);
            moveNext(row);
        }
    }
    return newM;
}

Matrix scalarMult(double x, Matrix A) {
    Matrix newM = newMatrix(A->size);
    for (int i = 1; i <= A->size; i++) {
        List row = A->rows[i];
        moveFront(row);
        while (index(row) >= 0) {
            Entry E = (Entry)get(row);
            changeEntry(newM, i, E->col, x * E->value);
            moveNext(row);
        }
    }
    return newM;
}

Matrix sum(Matrix A, Matrix B) {
    if(size(A) != size(B)) {
        printf("Matrix error: calling sum() on matrices of different sizes.\n");
        exit(EXIT_FAILURE);
    }

    Matrix result = newMatrix(size(A));
    for (int i = 1; i <= size(A); i++) {
        List rowA = A->rows[i];
        List rowB = B->rows[i];
        
        List sumRow = addLists(rowA, rowB);
        
        moveFront(sumRow);
        while (index(sumRow) >= 0) {
            Entry e = (Entry)get(sumRow);
            changeEntry(result, i, e->col, e->value); // Add or change the entry regardless of its value
            moveNext(sumRow);
            freeEntry(&e); // Free the Entry after use
        }
        freeList(&sumRow); 
    }
    return result;
}


Matrix diff(Matrix A, Matrix B) {
    if(size(A) != size(B)) {
        printf("Matrix error: calling diff() on matrices of different sizes.\n");
        exit(EXIT_FAILURE);
    }

    Matrix result = newMatrix(size(A));
    for (int i = 1; i <= size(A); i++) {
        List rowA = A->rows[i];
        List rowB = B->rows[i];
        List diffRow = subtractLists(rowA, rowB);

        for (moveFront(diffRow); index(diffRow) >= 0; moveNext(diffRow)) {
            Entry e = (Entry)get(diffRow);
            changeEntry(result, i, e->col, e->value);
        }
        freeList(&diffRow); // Free the List after use
    }
    return result;
}


double vectorDot(List P, List Q) {
    moveFront(P);
    moveFront(Q);
    double dotProduct = 0.0;
    while (index(P) >= 0 && index(Q) >= 0) {
        Entry p = (Entry)get(P);
        Entry q = (Entry)get(Q);
        if (p->col < q->col) {
            moveNext(P);
        } else if (p->col > q->col) {
            moveNext(Q);
        } else { 
            dotProduct += p->value * q->value;
            moveNext(P);
            moveNext(Q);
        }
    }
    return dotProduct;
}

Matrix product(Matrix A, Matrix B) {
    if(size(A) != size(B)) {
        printf("Matrix error: calling product() on matrices of different sizes.\n");
        exit(EXIT_FAILURE);
    }

    Matrix result = newMatrix(size(A));
    Matrix B_transposed = transpose(B);

    for (int i = 1; i <= size(A); i++) {
        for (int j = 1; j <= size(B_transposed); j++) {
            double dotProduct = vectorDot(A->rows[i], B_transposed->rows[j]);
            if(dotProduct != 0) { 
                changeEntry(result, i, j, dotProduct);
            }
        }
    }
    
    freeMatrix(&B_transposed);
    return result;
}

void printMatrix(FILE* out, Matrix M) {
    for (int i = 1; i <= M->size; i++) {
        List row = M->rows[i];
        if (length(row) > 0) {
            fprintf(out, "%d: ", i);
            moveFront(row);
            while (index(row) >= 0) {
                Entry E = (Entry)get(row);
                fprintf(out, "(%d, %.1f) ", E->col, E->value);
                moveNext(row);
            }
            fprintf(out, "\n");
        }
    }
}
