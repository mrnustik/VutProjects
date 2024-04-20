//
// Created by Michal Mrnustik on 26/02/2017.
//

#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>


#ifdef SERVER
#define SERVER_ERROR_INVALID_ARGUMENTS  1
#define SERVER_ERROR_NOT_A_DIRECTORY    2
#endif

#ifdef CLIENT
#define CLIENT_ERROR_INVALID_ARGUMENTS  1
#endif

void exitError(const char* message, int code);

#endif //ERROR_H
