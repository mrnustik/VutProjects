//
// Created by Michal Mrnustik on 26/02/2017.
//

#include "error.h"


void exitError(const char* message, int code)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(code);
}