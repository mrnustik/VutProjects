//
// Created by Michal Mrnustik on 28/02/2017.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

#define DEBUG true

void logInfo(const char* category, const char* message);
void logWarning(const char* category, const char* warning);
void logError(const char* category, const char* error);

#endif //LOGGER_H
