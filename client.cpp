#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "Library/Files.h"
#include "Library/Operation.h"
#include "Library/Logger.h"
#include "Library/Memory.h"

typedef struct {
    OperationType operation;
    string hostname;
    int portNumber;
    string remotePath;
    string localPath;
} tArguments;

string buildHttpRequest(tArguments* arguments)
{
    string request = operationToHTTPMethod(arguments->operation);
    string accept = operationToAccept(arguments->operation);
    string contentLength = getFileContentLength(arguments->localPath);
    string typeQuery = operationToTypeQuery(arguments->operation);

    request += " " + arguments->remotePath + typeQuery + " HTTP/1.1\r\n";
    request += "Host: "+ arguments->hostname +"\r\n";
    request += "Accept: " + accept + "\r\n";
    request += "Accept-Encoding: identity\r\n";
    if(arguments->operation == FILE_UPLOAD)
    {
        request += "Content-Type: application/octet-stream\r\n";
        request += "Content-Length: " +contentLength + "\r\n";
    }
    request += "\r\n";

    return request;
}

int parseArguments(int argc, char* argv[], tArguments** pArguments)
{
    if (argc < 3 || argc > 4) //invalid arguments count
    {
        return -1;
    }


    *pArguments = (tArguments *) mMalloc(sizeof(tArguments));

    char *opString = argv[1];

    OperationType operationType = parseOperation(opString);

    if (operationType == INVALID)
    {
        return -1;
    }

    (*pArguments)->operation = operationType;

    string address = argv[2];

    unsigned long protocolEnd = address.find("://");
    if (protocolEnd == string::npos)
        return -1;

    unsigned long hostnameEnd = address.find(":", protocolEnd + 3);
    if (hostnameEnd == string::npos)
        return -1;

    unsigned long portEnd = address.find("/", hostnameEnd);
    if (portEnd == string::npos)
        return -1;

    string hostname = address.substr(protocolEnd + 3, hostnameEnd - protocolEnd - 3);

    int port = stoi(address.substr(hostnameEnd + 1, portEnd - hostnameEnd - 1));

    string remotePath = address.substr(portEnd);

    (*pArguments)->portNumber = port;
    (*pArguments)->hostname = hostname;
    (*pArguments)->remotePath = remotePath;

    if(operationType == FILE_UPLOAD)
    {
        if(argc != 4)
        {
            logError("Arguments","Trying to upload but missing file.");
            return -1;
        }
        else
        {
            string localFile = argv[3];
            FILE* file = std::fopen(localFile.c_str(), "r");
            if(file == NULL)
            {
                logError("Arguments", "Invalid file path.");
                return -1;
            }
            else
            {
                (*pArguments)->localPath = localFile;
                fclose(file);
            }
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    memoryInit();

    tArguments* arguments = NULL;

    int valid_args = parseArguments(argc, argv, &arguments);

    if(valid_args < 0)
    {
        logError("Arguments", "Invalid arguments.");
        exit(EXIT_FAILURE);
    }

    logInfo("Networking", "Creating socket.");
    int clientSocket;
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    logInfo("Networking", "Socket created.");

    if (clientSocket == -1)
    {
        logError("Networking", "Could not create socket.");
    }

    logInfo("Networking", "Getting host by name.");
    struct sockaddr_in serv_addr;
    hostent* server = gethostbyname(arguments->hostname.c_str());
    if (server == NULL) {
        logError("Networking", "No such hostname");
        exit(EXIT_FAILURE);
    }
    logInfo("Networking", "Host found.");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(arguments->portNumber);

    if(connect(clientSocket, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        logError("Networking", "Could not connect to server.");
        exit(EXIT_FAILURE);
    }



    string message =  buildHttpRequest(arguments);
    if( send(clientSocket , message.c_str() , message.length() , 0) < 0)
    {
        logError("Networking", "Sending failed.");
        exit(EXIT_FAILURE);
    }

    if(arguments->operation == FILE_UPLOAD)
    {
        int bytesRead = 0;
        char fileBuffer[256];
        bzero(fileBuffer, 256);

        FILE* file = fopen(arguments->localPath.c_str(), "r");
        while((bytesRead = fread(fileBuffer, sizeof(char), 256, file)) > 0)
        {
            if(send(clientSocket, fileBuffer, bytesRead, 0) < 0)
            {
                logError("File send","Unable to send data.");
                break;
            }
            bzero(fileBuffer, 256);
        }
    }

    char buffer[1024];
    int n;
    string body = string();
    string header = string();
    string tmp = string();
    HttpRequest* request = NULL;
    int err = 0;
    int contentLength = 0;
    int contentRead = 0;
    while((n = read(clientSocket, buffer, 1024))> 0)
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
                if(request == NULL)
                {
                    err = -10;
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

        }
    }



    close(clientSocket);

    logInfo("Networking", "Closing socket.");
    close(clientSocket);
    logInfo("Networking", "Socket closed.");
    memoryDestroy();
    return 0;
}
