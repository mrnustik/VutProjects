

#ifndef HTTP_H
#define HTTP_H

using namespace std;

#include <string>

typedef enum {
    FIL,
    DIRECTORY
} Type;

typedef struct {
    string userName;
    string path;
    Type type;
} Url;

typedef enum {
    HTTP_OK,
    HTTP_CONFLICT,
    HTTP_FORBIDEN,
    HTTP_INVALID_REQUEST,
    HTTP_NOT_FOUND
} HttpResponseCode
;
#include "Operation.h"
#include "Files.h"

typedef enum {
    GET,
    POST,
    PUT,
    DELETE
} HttpMethod;


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

typedef struct {
    HttpResponseCode code;
    string date;
    string contentType;
    int contentLength;
} HttpResponse;

OperationType parseOperation(HttpMethod method, Type type);

HttpRequest* httpRequestFromString(string str);

HttpResponse* httpResponseFromString(string header);

string buildHttpRequest(OperationType operation, string localPath, string remotePath, string hostname);

string buildHttpResponse(HttpResponseCode code, string contentType , unsigned long contentLength);

int httpResponseCodeToInt(HttpResponseCode code);


#endif