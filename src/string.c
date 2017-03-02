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
    string->allocated = STRING_INIT_SIZE;
    string->used = 0;
    return string;
}


void stringAppend(tString* string, char* data)
{

}

int stringFind(tString* string, const char* find)
{

}

void stringErase(tString* string)
{

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