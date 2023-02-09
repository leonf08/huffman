#include <stdio.h>

#include "TextFileHandler.h"

#define MAX_SIZE 256U


int main(int argc, char **argv)
{
    char filename[] = "temp.txt";
    int hashmap[MAX_SIZE] = {0};

    HandleTextFile(filename, hashmap);

    for (int i = 0; i < MAX_SIZE; i++) {
        printf("%d-th element: %d\n", i, hashmap[i]);
    }
}