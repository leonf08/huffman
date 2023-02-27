/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "TextFileHandler.h"

/**** MAIN ********************************************************************/

int main(int argc, char **argv)
{
    switch (argc)
    {
        case 1:
        case 2:
        case 3:
            puts("Usage: huff [INPUTFILE] [-c][-x] [OUTPUTFILE]");            
            break;
        case 4:
            if (0 == strcmp(argv[2], "-c")) {
                archiveFile(argv[1], argv[3]);
            } else if (0 == strcmp(argv[2], "-x")) {
                unzipArchive(argv[1], argv[3]);
            }
            break;
        default:
            break;
    }

    return 0;
}

/**** GLOBAL FUNCTIONS DEFINITIONS ********************************************/

bool isMemoryAllocated(const void *const ptr)
{
    bool returnVal = true;
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory");
        returnVal = false;
    }

    return returnVal;
}