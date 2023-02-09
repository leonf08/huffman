#include <stdio.h>
#include <stdlib.h>

#include "TextFileHandler.h"


int HandleTextFile(const char filename[], int *hashmap)
{
    FILE *filePtr = fopen(filename, "r");
    if (filePtr == NULL) {
        perror("Error occured");
        return 1;
    }

    int c;
    while ((c = fgetc(filePtr)) != EOF)
    {
        hashmap[c]++;
    }

    return 0;
}