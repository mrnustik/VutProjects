//
// Created by Michal Mrnustik on 01/03/2017.
//

#include "string.h"
#include "memory.h"
#include "logger.h"
#include "error.h"

tString* stringInit()
{
    tString* string = mMalloc(sizeof(tString));
    if(string == NULL)
    {
        logWarning("String", "Failed memory allocation");
        exitError("Unable to allocate memory", EXIT_FAILURE);
    }
    string->pointer = mMalloc(sizeof(STRING_INIT_SIZE));
    if(string->pointer == NULL)
    {
        logWarning("String", "Failed memory allocation");
        exitError("Unable to allocate memory", EXIT_FAILURE);
    }
    bzero(string->pointer, STRING_INIT_SIZE);
    string->allocated = STRING_INIT_SIZE;
    string->used = 0;
    return string;
}


void stringAppend(tString* string, char* data)
{
    int dataLength = strlen(data);
    if(dataLength + string->used > string->allocated)
    {
        string->pointer = mRealloc(string->pointer, string->allocated + dataLength + STRING_INIT_SIZE);
        if(string->pointer == NULL)
        {
            logWarning("String", "Failed memory allocation");
            exitError("Unable to allocate memory", EXIT_FAILURE);
        }
    }
    strcat(string->pointer, data);
    string->used += dataLength;
}

int stringFind(tString* string, const char* find)
{

}

void stringErase(tString* string)
{
    bzero(string->pointer, string->allocated);
}

int substringPosition(char* string, const char* substring)
{
    char* substringPointer = strstr(string, substring);
    if(substringPointer == NULL)
    {
        return -1;
    }
    return (int) (string - substringPointer);
}