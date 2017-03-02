//
// Created by Michal Mrnustik on 01/03/2017.
//

#ifndef STRING_H
#define STRING_H

#include <string.h>
#include <memory.h>

#define STRING_INIT_SIZE 256

typedef struct tString{
    char* pointer;
    int allocated;
    int used;
} tString;

tString* stringInit();
void stringAppend(tString* string, char* data);
void stringErase(tString* string);
int stringFind(tString* string, const char* find);
int substringPosition(char* string, const char* substring);

#endif //FTREST_STRING_H
