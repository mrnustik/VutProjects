#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <cstring>

#include "Library/Directories.h"
#include "Library/Memory.h"
#include "Library/Logger.h"
#include "Library/Http.h"
#include "Library/Files.h"
#include "Library/Operation.h"

typedef struct
{
    string rootFolder;
    int port;
} tArguments;

void exit();

#define E_OK            200
#define E_INVALID       400
#define E_NOT_FOUND     404

int readParameter(char *flag, char *value, tArguments **pArguments)
{
    if (strcmp(flag, "-r") == 0)
    {
        (*pArguments)->rootFolder = value;
        logInfo("Arguments root folder", value);
    } else if (strcmp(flag, "-p") == 0)
    {
        (*pArguments)->port = atoi(value);
        logInfo("Arguments port", value);
    } else
    {
        return -1;
    }
    return 0;
}

int parseArguments(int argc, char *argv[], tArguments **pArguments)
{
    (*pArguments) = new tArguments;
    (*pArguments)->rootFolder = string();
    (*pArguments)->rootFolder = getCurrentDirectory();
    (*pArguments)->port = 80;

    int result = 0;
    if (argc == 1)
    {}
    else if (argc == 3)
    {
        readParameter(argv[1], argv[2], pArguments);
    } else if (argc == 5)
    {
        readParameter(argv[1], argv[2], pArguments);
        readParameter(argv[3], argv[4], pArguments);
    } else
    {
        result = -1;
    }

    DIR* dir = opendir((*pArguments)->rootFolder.c_str());
    if (dir)
    {
        closedir(dir);
    }
    else
    {
        logError("Directory", "Unable to open directory.");
        return -1;
    }

    return result;
}

int main(int argc, char *argv[])
{

    memoryInit();

    tArguments *arguments = NULL;

    int valid_args = parseArguments(argc, argv, &arguments);
    if (valid_args < 0)
    {
        logError("Arguments", "Invalid arguments");
        exit();
    }

    logInfo("Networking", "Opening server socket...");
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        logError("Networking", "Unable to create socket");
        exit();
    }
    logInfo("Networking", "Server socket opened.");

    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(arguments->port);

    logInfo("Networking", "Binding server socket.");

    if ((::bind(server_socket, (struct sockaddr *) &socketAddress, (socklen_t) sizeof(socketAddress))) != 0)
    {
        logError("Networking", "Unable to bind server socket");
        exit();
    }
    logInfo("Networking", "Server socket bound.");

    logInfo("Networking", "Starting listening for 1 input connection...");
    if (listen(server_socket, 1) < 0)
    {
        logError("Networking", "Unable to start listening on server socket");
        exit();
    }
    logInfo("Networking", "Listening successful.");

    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t incomingSocketLength = 0;

    while(true)
    {
        int incomingSocket = accept(server_socket, (struct sockaddr*) &clientAddress, &incomingSocketLength);
        if(incomingSocket > 0)
        {
            logInfo("Networking", "Socket incoming");

            char buffer[1024];
            int n;

            string body = string();
            string header = string();
            string tmp = string();
            HttpRequest* request = NULL;
            OperationResponse* result = NULL;
            int err = 0;
            int contentLength = 0;
            OperationType operation = INVALID;
            int contentRead = 0;
            while((n = read(incomingSocket, buffer, 1024))> 0)
            {    
                tmp.append(buffer, n);
                if(header.size() == 0)
                {
                    if(tmp.find("\r\n\r\n") != string::npos)
                    {
                        int headerIndex = tmp.find("\r\n\r\n");
                        header = tmp.substr(0, headerIndex + 4);
                        body = tmp.substr(headerIndex + 4);
                        contentRead += body.size();
                        tmp.erase();
                        request = httpRequestFromString(header);
                        if(request == NULL)
                        {
                            err = E_INVALID;
                            break;
                        }
                        else
                        {
                            contentLength = request->contentLength;
                        }
                    }
                }
                else 
                {
                    contentRead += n;
                    body.append(buffer, n);
                }
                bzero(buffer, 1024);
                if(contentRead == contentLength) break;
            }
            if(request != NULL) {
                operation = request->operation;
                switch (request->operation) {
                    case DIR_MAKE:
                        result = operationMakeDirectory(arguments->rootFolder, request->url);
                        break;
                    case DIR_REMOVE:
                        result = operationRemoveDirectory(arguments->rootFolder, request->url);
                        break;
                    case DIR_LIST:
                        result = operationListDirectory(arguments->rootFolder, request->url);
                        break;
                    case FILE_DELETE:
                        result = operationDeleteFile(arguments->rootFolder, request->url);
                        break;
                    case FILE_GET:
                        result = operationDownloadFile(arguments->rootFolder, request->url);
                        break;
                    case FILE_UPLOAD:
                        result = operationUploadFile(arguments->rootFolder, request->url, body);
                        break;
                }
            }
            else
            {
                //TODO build invalid request
            }

            string response = buildHttpResponse(result->httpCode, result->contentType, body.length());

            if(write(incomingSocket, response.c_str(), response.length()) < 0)
            {
                logError("Networking", "Could not write response to client.");
                close(incomingSocket);
                continue;
            }

            if(result->body.length() > 0)
            {
                if (write(incomingSocket, result->body.c_str(), result->body.length()) < 0)
                {
                    logError("Networking", "Could not write response body to client.");
                    close(incomingSocket);
                    continue;
                }
            }

            logInfo("Networking", "Comunication end. Closing socket.");
            close(incomingSocket);
            logInfo("Networking", "Client socket closed.");
        }
        else
        {
            logInfo("Networking", "Unable to accept socket");
        }
    }
    memoryDestroy();
    close(server_socket);

    return 0;
}

void exit() 
{
    memoryDestroy();
    exit(EXIT_FAILURE);
}