#include "TCPScanner.h"
#include "Logger.h"
#include <cstring>
#include <algorithm>
#include <netinet/ip.h>
#include <vector>

TcpScanner::TcpScanner(const Arguments* arguments): arguments(arguments) {
}


TcpScanner::~TcpScanner()
{
}

void TcpScanner::Scan(IpAddress address)
{
    char pcapBuffer[PCAP_ERRBUF_SIZE];
    pcap_if_t* devices,* device;
    pcap_findalldevs(&devices, pcapBuffer);
    std::vector<uint16_t > openPorts;
    srand(time(nullptr));
    for(device = devices; device != nullptr; device = device->next) {
        if(strcmp(device->name, "any") == 0) continue;
        if(!arguments->interfaceName.empty() && arguments->interfaceName != device->name) continue;

        Logger::Debug("TCP Scanner", "Send to device " + std::string(device->name));
        
        int socket = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(!TryBindSocketToInterface(socket, arguments->interfaceName)) continue;

        char buffer[256];
        tcphdr tcph = CreateTcpHeader();

        //SetNonBlocking(socket);

        iphdr iph;
        iph.ihl = 5;
        iph.version = 4;
        iph.tos = 0;
        iph.tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
        iph.id = htons (54321); 
        iph.frag_off = htons(16384);
        iph.ttl = 64;
        iph.protocol = IPPROTO_TCP;
        iph.check = 0; 
        iph.saddr = GetInAddrFromPcapDevice(device);
        iph.daddr = address.ToInAddr().s_addr;


        int one = 1;
        
        if (setsockopt (socket, IPPROTO_IP, IP_HDRINCL, &one, sizeof (one)) < 0)
        {
            Logger::Error("TCP Scanner", "Can't set socket to use my IP header", errno);
            continue;
        }
        
        short sourceportOffset = 50000;
        std::vector<int> openSockets;
        for(int i = 0; i < 656; i++)
        {
            if(arguments->portNumber == Arguments::AllPorts) {
                for (int port = 1 + i * 100; port <= 100 + i * 100; port++) {
                    if(port > 65535) break;
                    SendTcpSyn(socket, device, address, tcph, sourceportOffset, static_cast<uint16_t>(port), iph);
                }
            } else {
                SendTcpSyn(socket, device, address, tcph, sourceportOffset, static_cast<uint16_t>(arguments->portNumber), iph);
            }

            fd_set readFileDescriptors;
            bzero(&readFileDescriptors, sizeof(readFileDescriptors));
            FD_SET(socket, &readFileDescriptors);
            long lastValidTime = GetCurrentMilis();
            while (true) {
                //Initialize timeout
                struct timeval timeout;
                bzero(&timeout, sizeof(timeout));
                timeout.tv_sec = this->arguments->maxRtt / 1000;
                timeout.tv_usec = this->arguments->maxRtt % 1000;
                
                long current = GetCurrentMilis();
                if(current - lastValidTime > arguments->maxRtt){
                    break;
                }
                //Select if incoming messages
                struct tcphdr receivedHeader;
                bzero(&receivedHeader, sizeof(receivedHeader));
                auto result = select(socket + 1, &readFileDescriptors, NULL, NULL, &timeout);
                if (result <= 0) {
                    Logger::Debug("TCP Scanner", "Select timeout");
                    break;
                }

                //Receive incoming message
                ssize_t count = recv(socket, buffer, sizeof buffer, 0);
                if (count <= 0) {
                    continue;
                }
                if (count < sizeof receivedHeader) {
                    continue;
                }

                //Read the ip header
                auto *ip_header = (ip *) buffer;
                const int ipLength = ip_header->ip_hl << 2;
                //Read the TCP message
                memcpy(&receivedHeader, buffer + ipLength, sizeof receivedHeader);

                if (receivedHeader.ack == 1 && receivedHeader.syn == 1) {
                    uint16_t port = ntohs(receivedHeader.source);
                    uint16_t sourcePort = ntohs(receivedHeader.dest);
                    if(std::find(std::begin(openPorts), std::end(openPorts), port) == openPorts.end()) {
                        lastValidTime = GetCurrentMilis();
                        openPorts.push_back(port);
                        SendTcpReset(socket, device,address, port, sourcePort);
                        if(arguments->portNumber == Arguments::AllPorts)
                            std::cout << address.ToString() << " TCP " << port << std::endl;
                        else {
                            std::cout << address.ToString() << std::endl;
                            break;
                        }
                    }
                } else if(receivedHeader.ack == 1 && receivedHeader.rst == 1){
                    uint16_t port = ntohs(receivedHeader.source);
                    uint16_t sourcePort = ntohs(receivedHeader.dest);
                    SendTcpReset(socket, device,address, port, sourcePort);
                }
            }
        }
        CloseSocket(socket);
    }
}

void
TcpScanner::SendTcpSyn(int socket, pcap_if_t *device, IpAddress &address, tcphdr &tcph, short sourceportOffset,
                       uint16_t port, iphdr &iph) {
    char buffer[256];
    short sourcePort = rand() % 65535;
    tcph.source = sourcePort;
    //tcph.source = htons (sourceportOffset + port % 100);
    tcph.dest = htons(port);
    TCPPseudoHeader pseudoHeader = CreatePseudoHeader(device, address, tcph);
    tcph.check = Checksum(&pseudoHeader, sizeof(TCPPseudoHeader));
    struct sockaddr_in socketAddress;
    bzero(&socketAddress, sizeof(socketAddress));
    GetSocketAddress(address, port, &socketAddress);
    memcpy(buffer, &iph, sizeof(iph));
    memcpy(buffer + sizeof(iph), &tcph, sizeof(tcph));
    sendto(socket, buffer, sizeof(tcph) + sizeof(iph), 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
    tcph.check = 0;
}

tcphdr TcpScanner::CreateTcpHeader() const {
    tcphdr tcph;
    bzero(&tcph, sizeof(tcph));
    //TCP Header
    tcph.source = htons ( 54826);
    tcph.seq = htonl(1105024978);
    tcph.ack_seq = 0;
    tcph.doff = sizeof(struct tcphdr) / 4;      //Size of tcp header
    tcph.fin=0;
    tcph.syn=1;
    tcph.rst=0;
    tcph.psh=0;
    tcph.ack=0;
    tcph.urg=0;
    tcph.window = htons ( 14600 );  // maximum allowed window size
    tcph.check = 0; //if you set a checksum to zero, your kernel's IP stack should fill in the correct checksum during transmission
    tcph.urg_ptr = 0;
    return tcph;
}

TCPPseudoHeader TcpScanner::CreatePseudoHeader(pcap_if *pIf, IpAddress address,  tcphdr tcpHeader) {
    TCPPseudoHeader pseudoHeader;
    bzero(&pseudoHeader, sizeof(TCPPseudoHeader));
    pseudoHeader.sourceAddress = GetInAddrFromPcapDevice(pIf);
    pseudoHeader.destinationAddress = address.ToInAddr().s_addr;
    pseudoHeader.placeholder = 0;
    pseudoHeader.protocol = IPPROTO_TCP;
    pseudoHeader.tcpLength = htons(sizeof(tcpHeader));
    memcpy(&pseudoHeader.tcp, &tcpHeader, sizeof(tcpHeader));
    return  pseudoHeader;
}

unsigned int TcpScanner::GetInAddrFromPcapDevice(pcap_if *device) {
    for(pcap_addr_t *a=device->addresses; a!=NULL; a=a->next) {
        if(a->addr->sa_family == AF_INET)
        {
            return ((struct sockaddr_in*) a->addr)->sin_addr.s_addr;
        }
    }
    return 0;
}

void TcpScanner::SendTcpReset(int socket, pcap_if_t *device,  IpAddress address, uint16_t destinationPort, uint16_t sourcePort) {
    char buffer[256];
    iphdr ipHeader;
    ipHeader.ihl = 5;
    ipHeader.version = 4;
    ipHeader.tos = 0;
    ipHeader.tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
    ipHeader.id = htons (54321); //Id of this packet
    ipHeader.frag_off = htons(16384);
    ipHeader.ttl = 64;
    ipHeader.protocol = IPPROTO_TCP;
    ipHeader.check = 0;      //Set to 0 before calculating checksum
    ipHeader.saddr = GetInAddrFromPcapDevice(device);
    ipHeader.daddr = address.ToInAddr().s_addr;
    tcphdr tcpHeader = CreateTcpHeader();
    tcpHeader.syn = 0;
    tcpHeader.rst = 1;
    tcpHeader.source = htons (sourcePort);
    tcpHeader.dest = htons(destinationPort);
    struct sockaddr_in socketAddress;
    bzero(&socketAddress, sizeof(socketAddress));
    GetSocketAddress(address, destinationPort, &socketAddress);
    memcpy(buffer, &ipHeader, sizeof(ipHeader));
    memcpy(buffer + sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader));
    sendto(socket, buffer, sizeof(tcpHeader) + sizeof(ipHeader), 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));

}

