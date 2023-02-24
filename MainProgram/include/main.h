#ifndef MAIN_H
#define MAIN_H

/**** INCLUDES ****************************************************************/

#include <stdbool.h>

/**** MACROS ******************************************************************/

#define MAX_CHARS 256U

/**** TYPEDEFS ****************************************************************/

typedef struct {
    unsigned int numOfUniqueChars;
    unsigned int freq[MAX_CHARS];
} tableOfFrequencies_t;

/**** GLOBAL FUNCTIONS DECLARATIONS *******************************************/

bool isMemoryAllocated(const void *const ptr);


#endif /* MAIN_H */