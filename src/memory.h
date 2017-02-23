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
    tAllocUnit* first;
    tAllocUnit* last;
} tGlobalMemory;

tGlobalMemory* memory;

void memoryInit();

void* mMalloc(long size);

void* mRealloc(void* oldPointer, long size);

void memoryDestroy();

#endif //MEMORY_H
