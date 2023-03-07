/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "TextFileHandler.h"

/**** MAIN ********************************************************************/

/*
Main function: accepts two option [-c][-x]
Usage example: huff [INPUTFILE] [-c][-x] [OUTPUTFILE]
Options:
    -c - compress INPUTFILE (text file) into OUTPUTFILE (archive)
    -x - unzip INPUTFILE (archive) into OUTPUTFILE (text file)

@param argc: Number of arguments from the command line
@param argv: Array of arguments from the command line

@return 0
 */

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

/* 
This function checks if memory is dynamically allocated 

@param ptr: Address of memory that should be allocated

@return Bool value: true if memory was successfully allocated, otherwise false
*/

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