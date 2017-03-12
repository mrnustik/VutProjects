#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

#include "Library/Directories.h"
#include "Library/Memory.h"
#include "Library/Logger.h"
#include "Library/Http.h"
#include "Library/Files.h"

typedef struct
{
    string rootFolder;
    int port;
} tArguments;

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
    (*pArguments) = (tArguments*) mMalloc(sizeof(tArguments));
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
        exit(EXIT_FAILURE);
    }

    logInfo("Networking", "Opening server socket...");
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        logError("Networking", "Unable to create socket");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }
    logInfo("Networking", "Server socket bound.");

    logInfo("Networking", "Starting listening for 1 input connection...");
    if (listen(server_socket, 1) < 0)
    {
        logError("Networking", "Unable to start listening on server socket");
        exit(EXIT_FAILURE);
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
            int err = 0;
            int contentLength = 0;
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
            
            switch(request->operation)
            {
                case DIR_MAKE:
                    err = createDirectory(arguments->rootFolder, request);
                    break;
                case DIR_REMOVE:
                    err = deleteDirectory(arguments->rootFolder, request);
                    break;
                case DIR_LIST:
                    //err = listDirectory(arguments->rootFolder, request);
                    break;
                case FILE_DELETE:
                    err = deleteFile(arguments->rootFolder, request);
                    break;
                case FILE_GET:
                    //err = getFile(request);
                    break;
                case FILE_UPLOAD:
                    err = writeFile(arguments->rootFolder, request, body);
                    break;
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

    close(server_socket);

    return 0;
}