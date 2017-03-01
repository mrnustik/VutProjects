//
// Created by Michal Mrnustik on 01/03/2017.
//

#include "http.h"
#include "logger.h"


HttpRequest* httpFromRequestString(char* requestString)
{
    HttpRequest* request = mMalloc(sizeof(HttpRequest));

    if(request == NULL)
    {
        logWarning("HTTP parsing:", "Unable to allocate memory for request.");
        return NULL;
    }

    if(substringPosition(requestString, "GET") == 0)
    {
        request->method = GET;
    }
    else if(substringPosition(requestString, "PUT") == 0)
    {
        request->method = PUT;
    }
    else if(substringPosition(requestString, "DELETE") == 0)
    {
        request->method = DELETE;
    }
    else
    {
        logWarning("HTTP parsing", "Unable to parse HTTP method.");
        return NULL;
    }
}
