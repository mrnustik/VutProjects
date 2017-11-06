#include "UdpScanner.h"
#include "Logger.h"
#include <cstring>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <vector>
#include <algorithm>


UdpScanner::UdpScanner(const Arguments* arguments) : arguments(arguments)
{
}

UdpScanner::~UdpScanner()
{
}


void UdpScanner::Scan(IpAddress &address) {
    std::vector<unsigned short> ports;

    int icmpSocket = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int udpSocket = OpenSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SetNonBlocking(udpSocket);

    char buffer[1024];
    struct sockaddr_in socketAddress;
    bzero(&socketAddress, sizeof(socketAddress));

    for (unsigned short port = 1; port < 65535; port++) {
        if (arguments->portNumber != Arguments::AllPorts && arguments->portNumber != port) continue;
        GetSocketAddress(address, port, &socketAddress);
        ports.push_back(port);
        sendto(udpSocket, "ahoj", 5, 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));


        timeval timeout;
        bzero(&timeout, sizeof(timeout));
        timeout.tv_sec = arguments->maxRtt / 1000;
        timeout.tv_usec = arguments->maxRtt % 1000;

        fd_set readFileDescriptors{};
        FD_ZERO(&readFileDescriptors);
        FD_SET(icmpSocket, &readFileDescriptors);

        bzero(buffer, sizeof(buffer));
        if (select(icmpSocket + 1, &readFileDescriptors, nullptr, nullptr, &timeout) > 0) {
            recv(icmpSocket, &buffer, sizeof(buffer), 0);
        } else {
            Logger::Debug("UDP scan", "Select timeout");
            continue;
        }

        struct ip *iphdr = (struct ip *) buffer;
        unsigned int iplen = iphdr->ip_hl << 2;
        struct icmp *icmp = (struct icmp *) (buffer + iplen);
        struct ip *iphdr2 = (struct ip *) (buffer + iplen + 8);
        unsigned int iplen2 = iphdr->ip_hl << 2;

        if (icmp->icmp_type == ICMP_UNREACH) {
            Logger::Debug("UDP Scan", "Icmp unreachable");
            struct udphdr *udp = (struct udphdr *) ((char *) iphdr2 + iplen2);
            ports.erase(std::remove(ports.begin(), ports.end(), port), ports.end());
        } else {
            Logger::Debug("UDP Scanner", "Other ICMP message");
        }
    }
    for (unsigned short &port : ports) {
        if (arguments->portNumber == Arguments::AllPorts)
            std::cout << address.ToString() << " UDP " << port << std::endl;
        else
            std::cout << address.ToString() << std::endl;
    }
    CloseSocket(icmpSocket);
    CloseSocket(udpSocket);
}

