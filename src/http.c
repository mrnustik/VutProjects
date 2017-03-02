//
// Created by Michal Mrnustik on 01/03/2017.
//

#include "http.h"
#include "logger.h"


HttpRequest* httpFromRequestString(tString* string)
{
    HttpRequest* request = mMalloc(sizeof(HttpRequest));

    if(request == NULL)
    {
        logWarning("HTTP parsing:", "Unable to allocate memory for request.");
        return NULL;
    }

    int position = 0;

    logInfo("HTTP parsing:","Parsing started");
    if(stringFind(string, "GET") == 0)
    {
        logInfo("HTTP parsing:","Found GET method");
        request->method = GET;
        position += 3;
    }
    if(stringFind(string, "PUT") == 0)
    {
        logInfo("HTTP parsing:","Found PUT method");
        request->method = PUT;
        position += 3;
    }
    else if(stringFind(string, "DELETE") == 0)
    {
        logInfo("HTTP parsing:","Found DELETE method");
        request->method = DELETE;
        position += 6;
    }
    else
    {
        logWarning("HTTP parsing", "Unable to parse HTTP method.");
        return NULL;
    }


}
