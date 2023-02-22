#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "TextFileHandler.h"
#include "HuffmanAlgorithm.h"


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

static void processInputFile(const FILE *inputFilePtr, const char *outputFileName)
{
    unsigned int ch;
    unsigned char buffer = 0;
    unsigned char *code;

    FILE *outputFilePtr = fopen(outputFileName, "wb");
    if (outputFilePtr == NULL) {
        fprintf(stderr, "Failed to open file %s", outputFileName);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(inputFilePtr)) != EOF) {
        unsigned int bit_count  = 0;
        code = getCodeForChar(ch);
        while (*code != '\0') {
            *code = *code - '0';
            buffer |= (*code << (7 - bit_count));
            code++;
            bit_count++;

            if (bit_count == 8) {
                writeCompressedDataInFile(outputFilePtr, buffer);
                bit_count = 0, buffer = 0;
            }
        }

        if (bit_count > 0) {
            writeCompressedDataInFile(outputFilePtr, buffer);
            buffer = 0;
        }
    }
}

static void writeCompressedDataInFile(const FILE *outputFilePtr, unsigned char buffer)
{
    size_t numOfBytes = fwrite(&buffer, sizeof(unsigned char), 1, outputFilePtr);
    if (numOfBytes != 1) {
        fprintf(stderr, "Writing error");
        exit(EXIT_FAILURE);
    }
}

void archiveFile(const char *inputFileName, const char *outputFileName)
{
    FILE *inputFilePtr = fopen(inputFileName, "r");
    if (inputFilePtr == NULL) {
        fprintf(stderr, "Failed to open file %s", inputFileName);
        exit(EXIT_FAILURE);
    }

    tableOfFrequencies_t *freqTable = buildFreqTable(inputFilePtr);
    compressData(freqTable);

    rewind(inputFilePtr);

        
}