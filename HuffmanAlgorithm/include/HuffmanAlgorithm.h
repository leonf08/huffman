#ifndef HUFFMANALGORITHM_H
#define HUFFMANALGORITHM_H

/**** GLOBAL FUNCTIONS DECLARATIONS *******************************************/

unsigned char *getCodeForChar(unsigned char ch);
void compressData(const tableOfFrequencies_t *freqTable);

#endif /* HUFFMANALGORITHM_H */
