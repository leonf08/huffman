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

/*
Processes input text file, calculates the frequencies of 
each character in a file and puts them in an array

@param inputFilePtr: Pointer to FILE object associated with input text file

@return pointer to tableOfFrequencies_t data structure
*/

static tableOfFrequencies_t * buildFreqTable(FILE *inputFilePtr)
{
    tableOfFrequencies_t *freqTable = (tableOfFrequencies_t *)malloc(sizeof(tableOfFrequencies_t));
    if (!isMemoryAllocated(freqTable)) {
        exit(EXIT_FAILURE);
    }

    int c;
    /* Frequency of each character is put into array. Indexes of the array 
    correspond to numeric representation of a character */
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

/*
Processes input text file, extracts code for a character and writes the code 
into output file

@param inputFilePtr: Pointer to FILE object associated with input text file
@param outputFilePtr: Pointer to FILE object associated with output file with
compressed data
@param freqTable: Pointer to tableOfFrequencies_t data structure with 
frequencies for each character

@return no value
*/

static void processInputFile(FILE *inputFilePtr, FILE *outputFilePtr, tableOfFrequencies_t *freqTable)
{
    unsigned int ch;
    unsigned char buffer = 0, bitCount = 0;
    char *code;

    /* Write table of frequencies in the beginning of output file */
    writeCompressedDataInFile(outputFilePtr, freqTable, sizeof(tableOfFrequencies_t));

    /* Look through each character in input file and exctract Huffman code 
    for it. Codes are put into bytes and written into output file with 
    compressed data */
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

/*
Writes bytes with Huffman codes in a file

@param outputFilePtr: Pointer to FILE object associated with output file with
compressed data
@param data: Pointer to data to be written in a file
@param size: Size in bytes of type of data to be written in a file

@return no value
*/
static void writeCompressedDataInFile(FILE *outputFilePtr, void *data, size_t size)
{
    size_t count = 0;
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

/*
Exctracts table of frequencies for each character from archive file

@param inputFilePtr: Pointer to FILE object associated with input file with
compressed data

@return pointer to tableOfFrequencies_t data structure
*/

static tableOfFrequencies_t * extractFreqTable(FILE *inputFilePtr)
{
    tableOfFrequencies_t *freqTable = (tableOfFrequencies_t *)malloc(sizeof(tableOfFrequencies_t));
    if (!isMemoryAllocated(freqTable)) {
        exit(EXIT_FAILURE);
    }

    /* Read the beginning of the file and extract freqTable */
    size_t count = fread(freqTable, sizeof(tableOfFrequencies_t), 1, inputFilePtr);
    if (count != 1) {
        fprintf(stderr, "Reading error");
        exit(EXIT_FAILURE);
    }

    return freqTable;
}

/**** GLOBAL FUNCTION DEFINITIONS *********************************************/

/*
Makes data compression of input text file

@param inputFileName: Name of the input file (to be compressed)
@param outputFileName: Name of the output file (archive file)

@return no value
*/

void archiveFile(const char *inputFileName, const char *outputFileName)
{
    /* Open input text file to be compressed */
    FILE *inputFilePtr = fopen(inputFileName, "r");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    /* Open output archive file */
    FILE *outputFilePtr = fopen(outputFileName, "wb");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    /* Process input file and build table of frequencies */
    tableOfFrequencies_t *freqTable = buildFreqTable(inputFilePtr);
    /* Invoke huffman algorithm and generate codes for characters */
    compressData(freqTable);
    /* Set file pointer back to the beginning of the file */
    rewind(inputFilePtr);
    /* Process input file, encode characters and write encoded data 
    into archive file */
    processInputFile(inputFilePtr, outputFilePtr, freqTable);

    /* Deallocate memory and close all the files */
    free(freqTable);
    freeCodesTable();
    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

/*
Decompresses archive file and produces output test file

@param inputFileName: Name of the input file (archive file)
@param outputFileName: Name of the output file (text file with decompressed data)

@return no value
*/

void unzipArchive(const char *inputFileName, const char *outputFileName)
{
    /* Open input archive file */
    FILE *inputFilePtr = fopen(inputFileName, "rb");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    /* Open output text file with decompressed data */
    FILE *outputFilePtr = fopen(outputFileName, "w");
    if (inputFilePtr == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    /* Extract table of frequencies from the archive file */
    tableOfFrequencies_t *freqTable = extractFreqTable(inputFilePtr);
    
    /* Extract encoded data, decode them and write decoded data into 
    output text file */
    decompressData(freqTable, inputFilePtr, outputFilePtr);

    /* Deallocate memory and close all the files */
    free(freqTable);
    fclose(inputFilePtr);
    fclose(outputFilePtr);
}