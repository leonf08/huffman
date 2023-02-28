/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "TextFileHandler.h"
#include "HuffmanAlgorithm.h"

/**** LOCAL FUNCTION DECLARATIONS *********************************************/

static tableOfFrequencies_t * buildFreqTable(FILE *inputFilePtr);
static void processInputFile(FILE *inputFilePtr, FILE *outputFilePtr, tableOfFrequencies_t *freqTable);
static void writeCompressedDataInFile(FILE *outputFilePtr, void *data, size_t size);
static tableOfFrequencies_t * extractFreqTable(FILE *inputFilePtr);

/**** LOCAL FUNCTION DEFINITIONS **********************************************/

static tableOfFrequencies_t * buildFreqTable(FILE *inputFilePtr)
{
    tableOfFrequencies_t *freqTable = (tableOfFrequencies_t *)malloc(sizeof(tableOfFrequencies_t));
    if (!isMemoryAllocated(freqTable)) {
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(inputFilePtr)) != EOF)
    {
        if (freqTable->freq[c] == 0) {
            freqTable->numOfUniqueChars++;
        }
        freqTable->freq[c]++;
        freqTable->numOfAllChars++;
    }

    return freqTable;
}

static void processInputFile(FILE *inputFilePtr, FILE *outputFilePtr, tableOfFrequencies_t *freqTable)
{
    unsigned int ch;
    unsigned char buffer = 0, bitCount = 0;
    char *code;

    writeCompressedDataInFile(outputFilePtr, freqTable, sizeof(tableOfFrequencies_t));

    while ((ch = fgetc(inputFilePtr)) != EOF) {
        code = getCodeForChar(ch);
        while (*code != '\0') {
            buffer |= ((*code - '0') << (7 - bitCount));
            code++;
            bitCount++;

            if (bitCount == 8) {
                writeCompressedDataInFile(outputFilePtr, &buffer, sizeof(unsigned char));
                bitCount = 0, buffer = 0;
            }
        }
    }

    if (bitCount > 0) {
        writeCompressedDataInFile(outputFilePtr, &buffer, sizeof(unsigned char));
    }
}

static void writeCompressedDataInFile(FILE *outputFilePtr, void *data, size_t size)
{
    size_t count;
    if (size == sizeof(unsigned char)) {
        count = fwrite((unsigned char *)data, sizeof(unsigned char), 1, outputFilePtr);
    } else if (size == sizeof(tableOfFrequencies_t)) {
        count = fwrite((tableOfFrequencies_t *)data, sizeof(tableOfFrequencies_t), 1, outputFilePtr);
    }
    if (count != 1) {
        fprintf(stderr, "Writing error");
        exit(EXIT_FAILURE);
    }
}

static tableOfFrequencies_t * extractFreqTable(FILE *inputFilePtr)
{
    tableOfFrequencies_t *freqTable = (tableOfFrequencies_t *)malloc(sizeof(tableOfFrequencies_t));
    if (!isMemoryAllocated(freqTable)) {
        exit(EXIT_FAILURE);
    }

    size_t count = fread(freqTable, sizeof(tableOfFrequencies_t), 1, inputFilePtr);
    if (count != 1) {
        fprintf(stderr, "Reading error");
        exit(EXIT_FAILURE);
    }

    return freqTable;
}

/**** GLOBAL FUNCTION DEFINITIONS *********************************************/

void archiveFile(const char *inputFileName, const char *outputFileName)
{
    FILE *inputFilePtr = fopen(inputFileName, "r");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    FILE *outputFilePtr = fopen(outputFileName, "w");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    tableOfFrequencies_t *freqTable = buildFreqTable(inputFilePtr);
    compressData(freqTable);

    rewind(inputFilePtr);

    processInputFile(inputFilePtr, outputFilePtr, freqTable);

    free(freqTable);
    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

void unzipArchive(const char *inputFileName, const char *outputFileName)
{
    FILE *inputFilePtr = fopen(inputFileName, "rb");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    FILE *outputFilePtr = fopen(outputFileName, "w");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(inputFilePtr, 0, SEEK_END);
    size_t fileSize = ftell(inputFilePtr);
    rewind(inputFilePtr);

    tableOfFrequencies_t *freqTable = extractFreqTable(inputFilePtr);
    
    fileSize -= sizeof(tableOfFrequencies_t);
    decompressData(freqTable, inputFilePtr, fileSize, outputFilePtr);

    free(freqTable);
    fclose(inputFilePtr);
    fclose(outputFilePtr);
}