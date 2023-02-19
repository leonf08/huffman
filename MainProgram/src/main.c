#include <stdio.h>

#include "main.h"
#include "TextFileHandler.h"


tableOfFrequencies_t freqTable = {0, {0}};


int main(int argc, char **argv)
{
    char filename[] = "temp.txt";
    int hashmap[MAX_NUM_OF_CHARS] = {0};

    HandleTextFile(filename, hashmap);

    for (int i = 0; i < MAX_NUM_OF_CHARS; i++) {
        printf("%d-th element: %d\n", i, hashmap[i]);
    }
}