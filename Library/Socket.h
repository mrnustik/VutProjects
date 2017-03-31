//
// Created by Michal Mrnustik on 24.03.17.
//

#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H


#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

typedef enum {
    IPv4,
    IPv6
} AddressType;


class Socket {
public:
    Socket();

    ~Socket();

    int connectSocket(const std::string &address, const std::string port);

    int recieve(std::string &data);

    int send(const std::string &data);

    void closeSocket();

private:
    int socketNumber;
};


#endif //IPK_PROJ_2_CLIENT_SOCKET_H
