#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Library/Memory.h"
#include "Library/Logger.h"


typedef struct
{
    string rootFolder;
    int port;
} tArguments;

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
    (*pArguments)->rootFolder = ".";
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

            string message = string();

            while((n = read(incomingSocket, buffer, 1024))> 0)
                message.append(buffer, n);


            

            //TODO build response.

            logInfo("Networking", "Comunication end. Closing socket.");
            close(incomingSocket);
            logInfo("Networking", "Client socket closed.");
        }
        else
        {
            logInfo("Networking", "Unable to accept socket");
        }
        break;
    }

    close(server_socket);

    return 0;
}