
#include "Http.h"
#include "Memory.h"

Url* parseUrl(string str)
{
    Url* url = (Url*) mMalloc(sizeof(Url));

    unsigned long userNameEnd = str.find("/", 1);
    unsigned long queryTypeStart = str.find("?type=");
    if(userNameEnd == string::npos
            || queryTypeStart == string::npos)
    {
        return NULL;
    }

    url->userName = str.substr(1, userNameEnd - 1);
    url->path = str.substr(userNameEnd + 1, queryTypeStart - userNameEnd - 1);

    string type = str.substr(queryTypeStart + 6);
    if(type.compare("file") == 0)
    {
        url->type = FIL;
    }
    else if(type.compare("dir") == 0)
    {
        url->type = DIRECTORY;
    }
    else
    {
        return NULL;
    }


    return url;
}


string getParameterValue(string str, string paramName)
{
    unsigned long paramEnd = str.find(paramName);
    if(paramEnd == string::npos)
        return string();
    unsigned long lineEnd = str.find("\r\n", paramEnd);
    if(lineEnd == string::npos)
        return string();
    return str.substr(paramEnd + paramName.size(), lineEnd - paramEnd - paramName.size());
}


HttpRequest *httpRequestFromString(string str)
{
    HttpRequest* request = (HttpRequest*) mMalloc(sizeof(HttpRequest));

    int index = 0;
    if(str.find("GET") == 0)
    {
        request->method = GET;
        index += 4;
    }
    else if(str.find("PUT") == 0)
    {
        request->method = PUT;
        index += 4;
    }
    else if(str.find("POST") == 0)
    {
        request->method = POST;
        index += 5;
    }
    else if(str.find("DELETE") == 0)
    {
        request->method = DELETE;
        index += 7;
    }
    else
    {
        return NULL;
    }

    unsigned long versionStart = str.find(" HTTP/1.1\r\n");
    if(versionStart == string::npos)
        return NULL;

    string url = str.substr(index, versionStart - index);

    Url *url1 = parseUrl(url);
    if(url1 == NULL) return NULL;
    request->url = url1;

    request->date = getParameterValue(str, "Date: ");
    request->hostName = getParameterValue(str, "Host: ");
    request->accept = getParameterValue(str, "Accept: ");
    request->acceptEncoding = getParameterValue(str, "Accept-Encoding: ");
    string contentLength = getParameterValue(str, "Content-Length: ");
    try
    {
        request->contentLength = stoi(contentLength);
    }
    catch (std::invalid_argument invalidArgument)
    {
        request->contentLength = 0;
    }

    request->operation = parseOperation(request->method, request->url->type);

    return request;
}

OperationType parseOperation(HttpMethod method, Type type)
{
    switch(method)
    {
        case GET:
            if(type == FIL) return FILE_GET;
            else return DIR_LIST;
        case PUT:
            if(type == FIL) return FILE_UPLOAD;
            else return DIR_MAKE;
        case DELETE:
            if(type == FIL) return FILE_DELETE;
            else return DIR_REMOVE;
        default:
            break;
    }
    return INVALID;
}