//
// Created by Michal Mrnustik on 23/02/2017.
//

#include "operation.h"

OperationType parseOperation(char* opString)
{
    if(strcmp(opString, "del") == 0)
    {
        return DELETE;
    }
    else if(strcmp(opString, "get") == 0)
    {
        return DOWNLOAD;
    }
    else if(strcmp(opString, "put") == 0)
    {
        return UPLOAD;
    }
    else if(strcmp(opString, "lst") == 0)
    {
        return LIST;
    }
    else if(strcmp(opString, "mkd") == 0)
    {
        return MAKE_DIR;
    }
    else if(strcmp(opString, "rmd") == 0)
    {
        return REMOVE_DIR;
    }
    else
    {
        return INVALID;
    }
}