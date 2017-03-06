#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "src/error.h"
#include "src/memory.h"
#include "src/http.h"
#include "src/logger.h"


typedef struct
{
    char *rootFolder;
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
    (*pArguments) = mMalloc(sizeof(tArguments));
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
        exitError("Program has been started with invalid arguments", SERVER_ERROR_INVALID_ARGUMENTS);
    }

    logInfo("Networking", "Opening server socket...");
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        exitError("Unable to open server socket", EXIT_FAILURE);
    }
    logInfo("Networking", "Server socket opened.");

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(arguments->port);

    logInfo("Networking", "Binding server socket.");
    if (bind(server_socket, (struct sockaddr *) &socket_address, sizeof(socket_address)) != 0)
    {
        exitError("Unable to bind server socket", EXIT_FAILURE);
    }
    logInfo("Networking", "Server socket bound.");

    logInfo("Networking", "Starting listening for 1 input connection...");
    if (listen(server_socket, 1) < 0)
    {
        exitError("Unable to start listening on the server socket", EXIT_FAILURE);
    }
    logInfo("Networking", "Listening successful.");

    struct sockaddr_in client_addr;
    socklen_t clien_address_size = 0;
    char buffer[1024];
    int bytesRead = 0;
    bzero(buffer, 1024);
    logInfo("Networking", "Starting to accept on server socket");
    while (true)
    {
        int incoming_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clien_address_size);
        if (incoming_socket > 0)
        {

            tString* pom = stringInit();
            tString* header = stringInit();
            tString* body = stringInit();
            logInfo("Networking", "Client connected");
            logInfo("Networking", "Received incoming socket");
            while(true) {
                int position = -1;
                int n = read(incoming_socket, buffer, 1024);
                if(n <= 0)
                {
                    logError("Networking","Unable to read.");
                    close(incoming_socket);
                    break;
                }
                stringAppend(pom, buffer);
                if ((position = stringFind(pom, "\r\n\r\n")) > 0
                        && header->used == 0) {
                    header = stringSubstring(pom,0 ,  position + 4);
                    stringErase(pom);
                }

                if(header->used > 0)
                {
                    stringAppend(body, buffer);
                    stringErase(pom);
                }
                bzero(buffer, 1024);

            }
        }
        else
        {
            exitError("Unable to accept on server_socket", EXIT_FAILURE);
        }
        break;
    }

    logInfo("Networking", "Closing server socket.");
    close(server_socket);
    memoryDestroy();
    return 0;
}
