#include <stdio.h>

#include "main.h"
#include "TextFileHandler.h"



int main(int argc, char **argv)
{
    char filename[] = "temp.txt";
    int hashmap[MAX_CHARS] = {0};

    HandleTextFile(filename, hashmap);

    for (int i = 0; i < MAX_CHARS; i++) {
        printf("%d-th element: %d\n", i, hashmap[i]);
    }
}

bool isMemoryAllocated(const void *const ptr)
{
    bool returnVal = true;
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory");
        returnVal = false;
    }

    return returnVal;
}