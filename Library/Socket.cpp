//
// Created by Michal Mrnustik on 24.03.17.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Socket.h"


Socket::Socket() {
}

Socket::~Socket() {
    if(this->socketNumber > 0)
        close(socketNumber);
}


int Socket::connectSocket(const std::string &address, const std::string port) {
    struct addrinfo hints, *res=NULL;
    struct in6_addr serveraddr;

    int rc = 0;
    memset(&hints, 0x00, sizeof(hints));
    hints.ai_flags    = AI_NUMERICSERV;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    /********************************************************************/
    /* Check if we were provided the address of the server using        */
    /* inet_pton() to convert the text form of the address to binary    */
    /* form. If it is numeric then we want to prevent getaddrinfo()     */
    /* from doing any name resolution.                                  */
    /********************************************************************/
    rc = inet_pton(AF_INET, address.c_str(), &serveraddr);
    if (rc == 1)    /* valid IPv4 text address? */
    {
        hints.ai_family = AF_INET;
        hints.ai_flags |= AI_NUMERICHOST;
    }
    else
    {
        rc = inet_pton(AF_INET6, address.c_str(), &serveraddr);
        if (rc == 1) /* valid IPv6 text address? */
        {

            hints.ai_family = AF_INET6;
            hints.ai_flags |= AI_NUMERICHOST;
        }
    }

    rc = getaddrinfo(address.c_str(), port.c_str(), &hints, &res);
    if (rc != 0)
    {
        printf("Host not found --> %s\n", gai_strerror(rc));
        if (rc == EAI_SYSTEM)
            perror("getaddrinfo() failed");

    }

    this->socketNumber = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (this->socketNumber < 0)
    {
        perror("socket() failed");
    }

    rc = connect(this->socketNumber, res->ai_addr, res->ai_addrlen);
    if (rc < 0)
    {
        perror("connect() failed");
    }

    return rc;
}

int Socket::recieve(std::string &data) {
    char buffer[1024];
    unsigned long bytesRead = 0;
    if((bytesRead = read(this->socketNumber, buffer, 1024)) <= 0)
    {
        throw "Could not read from server.";
    }
    data.erase();
    data.append(buffer, bytesRead);
    return (int) bytesRead;
}

int Socket::send(const std::string &data) {
    int written = 0;
    if((written = write(this->socketNumber, data.c_str(), data.length())) < 0)
    {
        throw "Can not write to server.";
    }
    return written;
}

void Socket::closeSocket() {
    close(this->socketNumber);
}
