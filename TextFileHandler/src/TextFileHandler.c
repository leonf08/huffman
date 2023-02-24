/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "TextFileHandler.h"
#include "HuffmanAlgorithm.h"

/**** LOCAL FUNCTION DECLARATIONS *********************************************/

static tableOfFrequencies_t * buildFreqTable(FILE *inputFilePtr);
static void processInputFile(FILE *inputFilePtr, const char *outputFileName);
static void writeCompressedDataInFile(FILE *outputFilePtr, unsigned char buffer);


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
    }

    return freqTable;
}

static void processInputFile(FILE *inputFilePtr, const char *outputFileName)
{
    unsigned int ch;
    unsigned char buffer = 0;
    char *code;

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

    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

static void writeCompressedDataInFile(FILE *outputFilePtr, unsigned char buffer)
{
    size_t numOfBytes = fwrite(&buffer, sizeof(unsigned char), 1, outputFilePtr);
    if (numOfBytes != 1) {
        fprintf(stderr, "Writing error");
        exit(EXIT_FAILURE);
    }
}

/**** GLOBAL FUNCTION DEFINITIONS *********************************************/

void archiveFile(const char *inputFileName, const char *outputFileName)
{
    FILE *inputFilePtr = fopen(inputFileName, "r");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    tableOfFrequencies_t *freqTable = buildFreqTable(inputFilePtr);
    compressData(freqTable);

    rewind(inputFilePtr);

    processInputFile(inputFilePtr, outputFileName);        
}