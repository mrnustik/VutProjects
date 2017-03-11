//
// Created by Michal Mrnustik on 11/03/2017.
//

#include "Operation.h"

using namespace std;

OperationType parseOperation(string opString)
{
    if(opString.compare("del") == 0)
    {
        return FILE_DELETE;
    }
    else if(opString.compare( "get") == 0)
    {
        return FILE_GET;
    }
    else if(opString.compare( "put") == 0)
    {
        return FILE_UPLOAD;
    }
    else if(opString.compare( "lst") == 0)
    {
        return DIR_LIST;
    }
    else if(opString.compare( "mkd") == 0)
    {
        return DIR_MAKE;
    }
    else if(opString.compare( "rmd") == 0)
    {
        return DIR_REMOVE;
    }
    else
    {
        return INVALID;
    }
}

string operationToHTTPMethod(OperationType type)
{
    switch(type)
    {
        case DIR_MAKE:
        case FILE_UPLOAD:
            return "PUT";
        case DIR_REMOVE:
        case FILE_DELETE:
            return "DELETE";
        case DIR_LIST:
        case FILE_GET:
            return "GET";
        default:
            break;
    }
    return string();
}

string operationToAccept(OperationType type)
{
    switch(type)
    {
        case DIR_MAKE:
        case FILE_UPLOAD:
            return "application/octet-stream";
        case DIR_REMOVE:
        case FILE_DELETE:
        case DIR_LIST:
        case FILE_GET:
            return "application/javascript";
        default:
            break;
    }
    return string();
}

string operationToTypeQuery(OperationType type)
{
    switch(type)
    {
        case DIR_MAKE:
        case DIR_REMOVE:
        case DIR_LIST:
            return "?type=dir";
        case FILE_DELETE:
        case FILE_UPLOAD:
        case FILE_GET:
            return "?type=file";
        default:
            break;
    }
    return string();
}
