

#ifndef HTTP_H
#define HTTP_H

#include <string>

#include "Operation.h"

using namespace std;

typedef enum {
    GET,
    POST,
    PUT,
    DELETE
} HttpMethod;

typedef enum {
    FIL,
    DIRECTORY
} Type;

typedef struct {
    string userName;
    string path;
    Type type;
} Url;

typedef struct {
    OperationType operation;
    HttpMethod method;
    string date;
    string hostName;
    string accept;
    string acceptEncoding;
    string contentType;
    int contentLength;
    Url* url;
} HttpRequest;


OperationType parseOperation(HttpMethod method, Type type);

HttpRequest* httpRequestFromString(string str);


#endif