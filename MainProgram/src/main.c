/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include "main.h"
#include "TextFileHandler.h"

/**** MAIN ********************************************************************/

int main(int argc, char **argv)
{
    char filename[] = "temp.txt";
    archiveFile(filename, "archive");
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