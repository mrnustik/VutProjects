//
// Created by Michal Mrnustik on 23/02/2017.
//

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

typedef struct sAllocUnit{
    void* pointer;
    long size;
    struct sAllocUnit* next;
}tAllocUnit;

typedef struct {
    tAllocUnit* First;
    tAllocUnit* Last;
} tGlobalMemory;


void memoryInit();

void* mMalloc(unsigned long size);

void* mRealloc(void* oldPointer, unsigned long size);

void memoryDestroy();

#endif //MEMORY_H
