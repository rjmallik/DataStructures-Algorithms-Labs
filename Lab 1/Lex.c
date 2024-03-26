#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 256

int main(int argc, char * argv[]){

    // Check for correct number of command line arguments
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // Open files for reading and writing
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    if(in == NULL || out == NULL){
        fprintf(stderr, "Error: Unable to open file %s for reading or %s for writing\n", argv[1], argv[2]);
        exit(1);
    }

    // Count the number of lines in input file
    int count = 0;
    char temp[MAX_LEN];
    while(fgets(temp, MAX_LEN, in) != NULL) {
        count++;
    }
    rewind(in); // Reset file pointer to the beginning

    // Allocate memory for an array of strings
    char **array = malloc(count * sizeof(char*));
    if(array == NULL){
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }

    // Read lines from file into the array
    for(int i = 0; i < count; i++){
        array[i] = malloc(MAX_LEN * sizeof(char));
        fgets(array[i], MAX_LEN, in);
    }

    // Create a new List and perform insertion sort
    List L = newList();
    append(L, 0); // Start with the first index

    for(int i = 1; i < count; i++){
        moveFront(L);
        while(index(L) >= 0 && strcmp(array[get(L)], array[i]) < 0){
            moveNext(L);
        }
        if(index(L) == -1){
            append(L, i);
        } else {
            insertBefore(L, i);
        }
    }

    // Print the array in sorted order to the output file
    for(moveFront(L); index(L) >= 0; moveNext(L)){
        fprintf(out, "%s", array[get(L)]);
    }

    // Free memory and close files
    for(int i = 0; i < count; i++){
        free(array[i]);
    }
    free(array);
    freeList(&L);
    fclose(in);
    fclose(out);

    return 0;
}
