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
    string->pointer = mMalloc(sizeof(char) * STRING_INIT_SIZE);
    if(string->pointer == NULL)
    {
        logWarning("String", "Failed memory allocation");
        exitError("Unable to allocate memory", EXIT_FAILURE);
    }
    string->allocated = STRING_INIT_SIZE;
    string->used = 0;
    return string;
}


void stringAppend(tString* string, char* data)
{
    int dataLength = strlen(data);
    if(dataLength + string->used > string->allocated)
    {
        string->pointer = mRealloc(string->pointer, sizeof(char)*(string->allocated + dataLength + STRING_INIT_SIZE));
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
    return substringPosition(string->pointer, find);
}

tString* stringSubstring(tString* src, int from, int to)
{
    tString* ret = stringInit();
    if(ret->allocated < to - from)
    {
        ret->pointer = mRealloc(ret->pointer, sizeof(char) * (ret->allocated + to - from + STRING_INIT_SIZE));
        ret->allocated += to - from + STRING_INIT_SIZE;
    }
    strncpy(ret->pointer, src->pointer + from, to - from);
    ret->used = to - from;
    return ret;
}


void stringErase(tString* string)
{
    bzero(string->pointer, string->allocated);
    string->used = 0;
}

int substringPosition(char* string, const char* substring)
{
    char* substringPointer = strstr(string, substring);
    if(substringPointer == NULL)
    {
        return -1;
    }
    return (int) (substringPointer - string);
}