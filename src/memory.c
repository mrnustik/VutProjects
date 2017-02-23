//
// Created by Michal Mrnustik on 23/02/2017.
//

#include "memory.h"

void memoryInit()
{
    memory = malloc(sizeof(tGlobalMemory));
}

void* mMalloc(long size)
{
    tAllocUnit* unit = malloc(sizeof(tAllocUnit));
    unit->pointer = malloc(sizeof(size));
    unit->size = size;
    if(memory->first == NULL && memory->last == NULL)
    {
        memory->first = unit;
        memory->last = unit;
    }
    else
    {
        memory->last->next = unit;
        memory->last = unit;
    }
    return unit->pointer;
}

void* mRealloc(void* pointer, long size)
{
    tAllocUnit* unit = memory->first;
    while(unit->pointer != pointer)
    {
        unit = unit->next;
        if(unit == NULL) return NULL;
    }
    unit->pointer = realloc(unit->pointer, size);
    unit->size = size;
    return unit->pointer;
}

void memoryDestroy()
{
    tAllocUnit* unit = memory->first;
    while(unit != NULL)
    {
        free(unit->pointer);
        free(unit);
    }
    free(memory);
}