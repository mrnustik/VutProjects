//
// Created by Michal Mrnustik on 28/02/2017.
//

#include "logger.h"


void logInfo(const char* category, const char* message)
{
    if(DEBUG)
        printf("INFO %s: %s\n", category, message);
}

void logWarning(const char* category, const char* message)
{
    if(DEBUG)
        printf("WARNING %s: %s\n", category, message);
}

void logError(const char* category, const char* message)
{
    if(DEBUG)
        printf("ERROR %s: %s\n", category, message);
}