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
        case DIR_REMOVE:
        case FILE_DELETE:
        case DIR_LIST:
            return "application/javascript";
        case FILE_GET:
            return "application/octet-stream";
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
            return "?type=folder";
        case FILE_DELETE:
        case FILE_UPLOAD:
        case FILE_GET:
            return "?type=file";
        default:
            break;
    }
    return string();
}

OperationResponse *operationMakeDirectory(string rootFolder, Url* url) {
    OperationResponse* response = new OperationResponse;
    int result = createDirectory(rootFolder, url);

    if(result == CODE_OK) response->httpCode = HTTP_OK;
    else response->httpCode = HTTP_INVALID_REQUEST;

    response->contentType = "application/javascript";

    //TODO build json response

    return response;
}

OperationResponse *operationRemoveDirectory(string rootFolder, Url* url) {
    OperationResponse* response = new OperationResponse;
    int result = createDirectory(rootFolder, url);

    if(result == CODE_OK) response->httpCode = HTTP_OK;
    else if(result == CODE_DIR_NOT_FOUND) response->httpCode = HTTP_NOT_FOUND;
    else response->httpCode = HTTP_INVALID_REQUEST;

    response->contentType = "application/javascript";

    //TODO buld json response

    return nullptr;
}

OperationResponse *operationListDirectory(string rootFolder, Url* url) {
    OperationResponse* response = new OperationResponse;
    vector<string> directories;
    int result = 0;

    tie(result, directories) = getDirectoryContent(rootFolder, url);

    if(result == CODE_OK)
    {
        response->httpCode = HTTP_OK;
    }
    else if(result == CODE_DIR_NOT_FOUND)
    {
        response->httpCode = HTTP_NOT_FOUND;
    }
    else
    {
        response->httpCode = HTTP_INVALID_REQUEST;
    }

    //TODO build json response
    if(directories.size() != 0)
    {

    }

    return response;
}

OperationResponse *operationUploadFile(string rootFolder, Url* url, string body) {
    OperationResponse* response = new OperationResponse;
    int result = writeFile(rootFolder, url, body);

    if(result == CODE_OK) response->httpCode = HTTP_OK;
    else if(result == CODE_EXISTS) response->httpCode = HTTP_CONFLICT;
    else response->httpCode = HTTP_INVALID_REQUEST;

    response->contentType = "application/javascript";

    //TODO build json response

    return response;
}

OperationResponse *operationDownloadFile(string rootFolder, Url* url) {
    OperationResponse* response = new OperationResponse;
    int result = 0;
    string body = "";

    tie(result, body) = readFile(rootFolder, url);

    if(result == CODE_OK)
    {
        response->httpCode = HTTP_OK;
    }
    else if(result == CODE_EXISTS)
    {
        response->httpCode = HTTP_CONFLICT;
    }
    else
    {
        response->httpCode = HTTP_INVALID_REQUEST;
    }

    response->body = body;
    response->contentType = "application/octet-stream";

    return response;
}

OperationResponse *operationDeleteFile(string rootFolder, Url* url) {
    OperationResponse* response = new OperationResponse;
    int result = deleteFile(rootFolder, url);

    if(result == CODE_OK) response->httpCode = HTTP_OK;
    else if(result == CODE_FILE_NOT_FOUND) response->httpCode = HTTP_NOT_FOUND;
    else response->httpCode = HTTP_INVALID_REQUEST;

    response->contentType = "application/javascript";

    //TODO build json response

    return response;
}

OperationResponse *operationInvalid()
{
    OperationResponse* response = new OperationResponse;
    response->httpCode = HTTP_INVALID_REQUEST;
    response->body = "";
    response->contentType = "";
    return response;
}
