#ifndef MAIN_H
#define MAIN_H

#define MAX_NUM_OF_CHARS 256U

typedef struct {
    unsigned numOfUniqueChars;
    unsigned freq[MAX_NUM_OF_CHARS];
} tableOfFrequencies_t;

#endif /* MAIN_H */
