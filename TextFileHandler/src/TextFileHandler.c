#include <stdio.h>
#include <stdlib.h>


void HandleTextFile(const char filename[])
{
    FILE *filePtr = fopen(filename, "r");

    while (filePtr != EOF)
    {
        int c = fgetc(filePtr);
    }
}