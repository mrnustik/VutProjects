//
// Created by Michal Mrnustik on 11/03/2017.
//

#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include "Json.h"
using namespace std;
typedef enum {
    DIR_MAKE,
    DIR_REMOVE,
    DIR_LIST,
    FILE_UPLOAD,
    FILE_DELETE,
    FILE_GET,
    INVALID
} OperationType;

#include "Directories.h"
#include "Codes.h"
#include "Http.h"

typedef struct
{
    HttpResponseCode httpCode;
    string contentType;
    string body;
} OperationResponse;

OperationType parseOperation(string str);

string operationToHTTPMethod(OperationType type);

string operationToAccept(OperationType type);

string operationToTypeQuery(OperationType types);

OperationResponse* operationMakeDirectory(string rootFolder, Url* url);

OperationResponse* operationRemoveDirectory(string rootFolder, Url* url);

OperationResponse* operationListDirectory(string rootFolder, Url* url);

OperationResponse *operationUploadFile(string rootFolder, Url* url, string body);

OperationResponse* operationDownloadFile(string rootFolder, Url* url);

OperationResponse* operationDeleteFile(string rootFolder, Url* url);

OperationResponse* operationInvalid();

#endif //OPERATION_H
