#ifndef HUFFMANALGORITHM_H
#define HUFFMANALGORITHM_H

/**** GLOBAL FUNCTIONS DECLARATIONS *******************************************/

char *getCodeForChar(unsigned char ch);
void compressData(const tableOfFrequencies_t *freqTable);
void decompressData(const tableOfFrequencies_t *freqTable, FILE *inputFilePtr, size_t fileSize, FILE *outputFilePtr);

#endif /* HUFFMANALGORITHM_H */
