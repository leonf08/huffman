#include <stdio.h>

#include "TextFileHandler.h"

#define MAX_NUM_OF_CHARS 256U

typedef struct {
    unsigned numOfUniqueChars;
    unsigned freq[MAX_NUM_OF_CHARS];
} tableOfFrequencies_t;

tableOfFrequencies_t freqTable;


int main(int argc, char **argv)
{
    char filename[] = "temp.txt";
    int hashmap[MAX_SIZE] = {0};

    HandleTextFile(filename, hashmap);

    for (int i = 0; i < MAX_SIZE; i++) {
        printf("%d-th element: %d\n", i, hashmap[i]);
    }
}