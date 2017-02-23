#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    (*pArguments) = malloc(sizeof(tArguments));
    (*pArguments)->rootFolder = ".";
    (*pArguments)->port = 80;

    int result = 0;
    if(argc > 2)
    {
        result = readParameter(argv[1], argv[2], pArguments);
    }
    if(argc == 4)
    {
        result = readParameter(argv[3], argv[4], pArguments);
    }
    return result;
}

int main(int argc, char* argv[])
{
    tArguments* arguments = NULL;

    int valid_args = parseArguments(argc, argv, &arguments);
    if(valid_args < 0)
    {
        return -1;
    }

	return 0;
}
