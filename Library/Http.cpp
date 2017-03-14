
#include "Files.h"
#include "Http.h"


string getCurrentHttpDate()
{
    char buf[256];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return string(buf);
}


Url* parseUrl(string str)
{
    Url* url = new Url;

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


HttpResponse *httpResponseFromString(string header) {
    HttpResponse* response = new HttpResponse;
    if(header.find("HTTP/1.1 ") != 0)
    {
        return NULL;
    }

    if(header.find("200 OK\r\n", 8) != string::npos)
    {
        response->code = HTTP_OK;
    }
    else if(header.find("400 Bad Request\r\n", 8) != string::npos)
    {
        response->code = HTTP_INVALID_REQUEST;
    }
    else if(header.find("404 Not Found\r\n", 8) != string::npos)
    {
        response->code = HTTP_NOT_FOUND;
    }
    else if(header.find("401 Forbidden\r\n", 8) != string::npos)
    {
        response->code = HTTP_FORBIDEN;
    }
    else if(header.find("409 Conflict\r\n", 8) != string::npos)
    {
        response->code = HTTP_CONFLICT;
    }
    else
    {
        return NULL;
    }

    response->date = getParameterValue(header,  "Date: ");
    response->contentType = getParameterValue(header, "Content-Type: ");
    string contentLength = getParameterValue(header, "Content-Length: ");
    try
    {
        response->contentLength = stoi(contentLength);
    }
    catch (std::invalid_argument invalidArgument)
    {
        response->contentLength = 0;
    }


    return response;
}




HttpRequest *httpRequestFromString(string str)
{
    HttpRequest* request = new HttpRequest;

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


string buildHttpRequest(OperationType operation, string localPath, string remotePath, string hostname)
{
    string request = operationToHTTPMethod(operation);
    string accept = operationToAccept(operation);
    string contentLength = getFileContentLength(localPath);
    string typeQuery = operationToTypeQuery(operation);
    string dateString = getCurrentHttpDate();

    request += " " + remotePath + typeQuery + " HTTP/1.1\r\n";
    request += "Host: "+ hostname +"\r\n";
    request += "Date: " + dateString + "\r\n";
    request += "Accept: " + accept + "\r\n";
    request += "Accept-Encoding: identity\r\n";
    if(operation == FILE_UPLOAD)
    {
        request += "Content-Type: application/octet-stream\r\n";
        request += "Content-Length: " +contentLength + "\r\n";
    }
    request += "\r\n";

    return request;
}

string httpResponseCodeToString(const HttpResponseCode code)
{
    switch(code)
    {
        case HTTP_OK:
            return "OK";
        case HTTP_INVALID_REQUEST:
            return "Bad Request";
        case HTTP_NOT_FOUND:
            return "Not Found";
        case HTTP_CONFLICT:
            return "Conflict";
        case HTTP_FORBIDEN:
            return "Forbidden";
    }
    return "";
}

string buildHttpResponse(HttpResponseCode code, string contentType ,unsigned long contentLength)
{
    string httpCode = to_string(httpResponseCodeToInt(code));
    string date = getCurrentHttpDate();
    string httpCodeValue = httpResponseCodeToString(code);
    string contLength = to_string(contentLength);
    string response = "";

    response += "HTTP/1.1 " + httpCode + " " + httpCodeValue + "\r\n";
    response += "Date: " + date + "\r\n";
    //Sometimes we dont have a Content-Type
    if(contentType.size() > 0)
    {
        response += "Content-Type: " + contentType + "\r\n";
        response += "Content-Length: " + contLength + "\r\n";
        response += "Content-Encoding: identity\r\n";
    }
    response += "\r\n";
    return response;
}


int httpResponseCodeToInt(HttpResponseCode code)
{
    switch (code)
    {
        case HTTP_OK:
            return 200;
        case HTTP_INVALID_REQUEST:
            return 400;
        case HTTP_FORBIDEN:
            return 401;
        case HTTP_NOT_FOUND:
            return 404;
        case HTTP_CONFLICT:
            return 409;
        default:
            return 0;
    }
}
