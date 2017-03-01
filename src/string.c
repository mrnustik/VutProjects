//
// Created by Michal Mrnustik on 01/03/2017.
//

#include "string.h"


int substringPosition(char* string, const char* substring)
{
    char* substringPointer = strstr(string, substring);
    if(substringPointer == NULL)
    {
        return -1;
    }
    return (int) (string - substringPointer);
}