#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/error.h"
#include "src/memory.h"


typedef struct {
    char* rootFolder;
    int port;
} tArguments;

int readParameter(char* flag, char* value, tArguments **pArguments) {
    if(strcmp(flag, "-r") == 0)
    {
        (*pArguments)->rootFolder = value;
    }
    else if(strcmp(flag, "-p") == 0)
    {
        (*pArguments)->port = atoi(value);
    }
    else
    {
        return -1;
    }
    return 0;
}

int parseArguments(int argc, char* argv[], tArguments** pArguments)
{
    (*pArguments) = mMalloc(sizeof(tArguments));
    (*pArguments)->rootFolder = ".";
    (*pArguments)->port = 80;

    int result = 0;
    if(argc == 1)
    {

    }
    else if(argc == 3)
    {
        readParameter(argv[1], argv[2], pArguments);
    }
    else if(argc == 5)
    {
        readParameter(argv[1], argv[2], pArguments);
        readParameter(argv[3], argv[4], pArguments);
    }
    else
    {
        result = -1;
    }
    return result;
}

int main(int argc, char* argv[])
{
    memoryInit();

    tArguments* arguments = NULL;

    int valid_args = parseArguments(argc, argv, &arguments);
    if(valid_args < 0)
    {
        exitError("Program has been started with invalid arguments", SERVER_ERROR_INVALID_ARGUMENTS);
    }



    memoryDestroy();
	return 0;
}
