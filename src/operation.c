//
// Created by Michal Mrnustik on 23/02/2017.
//

#include "operation.h"

OperationType parseOperation(char* opString)
{
    if(strcmp(opString, "del"))
    {
        return DELETE;
    }
    else if(strcmp(opString, "get"))
    {
        return DOWNLOAD;
    }
    else if(strcmp(opString, "put"))
    {
        return UPLOAD;
    }
    else if(strcmp(opString, "lst"))
    {
        return LIST;
    }
    else if(strcmp(opString, "mkd"))
    {
        return MAKE_DIR;
    }
    else if(strcmp(opString, "rmd"))
    {
        return REMOVE_DIR;
    }
    else
    {
        return INVALID;
    }
}