#include "memory.h"

tGlobalMemory *globalMemory; //globalMemory memory struct

//================================================================================
// Memory initialization
//================================================================================


void memoryInit()
{
    globalMemory = malloc(sizeof(tGlobalMemory));
    globalMemory->First = NULL;
    globalMemory->Last = NULL;
}

//================================================================================
// Memory request calls
//================================================================================


void* mMalloc(unsigned long size)
{

    if(globalMemory->Last == NULL && globalMemory->First == NULL)
    {
        globalMemory->First = globalMemory->Last = malloc(sizeof(tAllocUnit));
    }
    else
    {
        globalMemory->Last->next = malloc(sizeof(tAllocUnit));
        globalMemory->Last = globalMemory->Last->next;
    }
    globalMemory->Last->next = NULL;
    globalMemory->Last->pointer = malloc(size);
    return globalMemory->Last->pointer; //return allocated pointer or null if not allocated
}

void* mRealloc(void* oldPointer, unsigned long size)
{
    tAllocUnit *unit = globalMemory->First;
    while(unit != NULL)
    {
        if(unit->pointer == oldPointer)
        {
            unit->pointer = realloc(unit->pointer, size);
            return unit->pointer;
        }
        unit = unit->next;
    }
    return NULL;
}



void memoryDestroy()
{
    tAllocUnit *unit = globalMemory->First;
    tAllocUnit *tmp;
    while(unit != NULL)
    {
        if(unit->pointer != NULL)
        {
            free(unit->pointer);
        }

        tmp = unit;
        unit = unit->next;
        free(tmp);
    }
    free(globalMemory);
}