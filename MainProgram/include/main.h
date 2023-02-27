#ifndef MAIN_H
#define MAIN_H

/**** INCLUDES ****************************************************************/

#include <stdbool.h>

/**** MACROS ******************************************************************/

#define MAX_CHARS 256U

/**** TYPEDEFS ****************************************************************/

typedef struct {
    unsigned int freq[MAX_CHARS];
    long long numOfAllChars;
    unsigned char numOfUniqueChars;
} tableOfFrequencies_t;

/**** GLOBAL FUNCTIONS DECLARATIONS *******************************************/

bool isMemoryAllocated(const void *const ptr);


#endif /* MAIN_H */