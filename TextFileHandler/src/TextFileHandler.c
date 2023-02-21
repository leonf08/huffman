#include <stdio.h>
#include <stdlib.h>


#include "main.h"
#include "TextFileHandler.h"


static tableOfFrequencies_t * buildFreqTable(const FILE *inputFilePtr)
{
    tableOfFrequencies_t *freqTable = (tableOfFrequencies_t *)malloc(sizeof(tableOfFrequencies_t));
    if (!isMemoryAllocated(freqTable)) {
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(inputFilePtr)) != EOF)
    {
        freqTable->freq[c]++;
    }

    return freqTable;
}

static size_t readFileInBuffer(const FILE *inputFilePtr, unsigned char *buffer, const long int fileSize)
{
    size_t count = fread(buffer, sizeof(unsigned char), fileSize, inputFilePtr);

    return count;
}

void archiveFile(const char filename[])
{
    FILE *inputFilePtr = fopen(filename, "r");
    if (inputFilePtr == NULL) {
        fprintf(stderr, "Failed to open file %s", filename);
        exit(1);
    }

    tableOfFrequencies_t *freqTable = buildFreqTable(inputFilePtr);

    long int fileSize = ftell(inputFilePtr);
    rewind(inputFilePtr);

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * fileSize);
    if (!isMemoryAllocated(buffer)) {
        exit(EXIT_FAILURE);
    }

    size_t count = readFileInBuffer(inputFilePtr, buffer, fileSize);
    if (count != fileSize) {
        fprintf(stderr, "Reading error");
        exit(EXIT_FAILURE);
    }

    
}