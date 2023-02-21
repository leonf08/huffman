#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define MAX_CHARS 256U

typedef struct {
    unsigned int numOfUniqueChars;
    unsigned int freq[MAX_CHARS];
} tableOfFrequencies_t;


bool isMemoryAllocated(const void *const ptr);


#endif /* MAIN_H */
