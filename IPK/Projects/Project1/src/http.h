//
// Created by Michal Mrnustik on 01/03/2017.
//

#ifndef HTTP_H
#define HTTP_H

#include "string.h"
#include "memory.h"
#include "logger.h"

#include <time.h>


enum HttpMethod
{
    GET,
    PUT,
    DELETE,
    UNDEFINED
};


typedef struct HttpRequest
{
    enum HttpMethod method;
    char* httpVersion;
    char* address;
    char* date;
    char* accept;
    char* contentType;
    int contentLength;
} HttpRequest;

typedef struct HttpResponse
{
    char* httpVersion;
    time_t date;
    int responseCode;
    char* contentType;
    int contentLength;

} HttpResponse;

HttpRequest* httpFromRequestString(tString* request);
HttpResponse* httpFromResponseString(tString* response);


#endif //HTTP_H
