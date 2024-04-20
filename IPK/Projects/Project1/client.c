#define CLIENT

#include <stdio.h>
#include <stdlib.h>
#include "src/operation.h"
#include "src/memory.h"
#include "src/error.h"

typedef struct {
    OperationType operation;
    char* remotePath;
    char* localPath;
} tArguments;

int parseArguments(int argc, char* argv[], tArguments** pArguments)
{
    if(argc < 2 || argc > 3) //invalid arguments count
    {
        return -1;
    }

    *pArguments = mMalloc(sizeof(tArguments));

    char * opString = argv[1];

    OperationType operationType = parseOperation(opString);

    if(operationType == INVALID)
    {
        return -1;
    }

    (*pArguments)->operation = operationType;
    (*pArguments)->remotePath = argv[2];

    if(operationType == UPLOAD)
    {
        if(argc != 3) return -1;
        else (*pArguments)->localPath = argv[3];
    }
    return 1;
}

int main(int argc, char* argv[])
{
    memoryInit();

    tArguments* arguments = NULL;

    int valid_args = parseArguments(argc, argv, &arguments);

    if(valid_args < 0)
    {
        exitError("Program has been started with invalid arguments", CLIENT_ERROR_INVALID_ARGUMENTS);
    }

    memoryDestroy();
    return 0;
}
