#include "ArpScanner.h"

#include "Logger.h"
#include "NetworkHelper.h"
#include <cstring>
#include <net/if.h>
#include <net/if_arp.h>
#include <netpacket/packet.h>   // struct sockaddr_ll
#include <net/ethernet.h>       // ETHER_ADD_LEN, ETH_P_*
#include <unistd.h>



ArpScanner::ArpScanner(const Arguments* arguments):arguments(arguments)
{
}

ArpScanner::~ArpScanner()
{
}


typedef struct {
	struct {	                
		uint8_t destinationMac[6];
		uint8_t sourceMac[6];
		uint8_t type[2];
	} eth;
	struct {
		uint16_t hardwareType;
		uint16_t protocolType;
		uint8_t hardwareLength;
		uint8_t protocolLength;
		uint16_t operationCode;
		uint8_t senderMac[6];
		uint8_t senderIp[4];
		uint8_t targetMac[6];
		uint8_t targetIp[4];
	} arp;
	uint8_t padding[36];
} arpEthPacket;

std::vector<IpAddress> ArpScanner::ScanNetwork(IpNetwork network, std::string adapter) {
    std::vector<IpAddress> addressVector;
	uint8_t macAddress[6];
	NetworkHelper::GetMacAddress(macAddress, adapter);
	struct sockaddr_ll device;
    bzero(&device, sizeof(device));
    int socket = OpenSocket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL) );
    if(!TryBindSocketToInterface(socket, adapter))return addressVector;
	device.sll_ifindex = if_nametoindex(adapter.c_str());
	memcpy(&device.sll_addr, macAddress, 6*sizeof(uint8_t));
    device.sll_family = AF_PACKET;
    device.sll_halen = 6;
    IpAddress deviceIp = NetworkHelper::GetDeviceAddress(adapter);

    arpEthPacket packet;
    bzero(&packet, sizeof(packet));
    uint8_t dstAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(packet.eth.destinationMac, dstAddress , sizeof(dstAddress));
    memcpy(packet.eth.sourceMac, macAddress, sizeof(macAddress));
    packet.eth.type[0] = ETH_P_ARP / 256;
    packet.eth.type[1] = ETH_P_ARP % 256;

    packet.arp.hardwareType  = htons( 1 );             // 1 == Ethernet
    packet.arp.protocolType  = htons(ETH_P_IP);
    packet.arp.hardwareLength   = sizeof(uint8_t) * 6;
    packet.arp.protocolLength   = sizeof(uint8_t) * 4;
    packet.arp.operationCode = htons( ARPOP_REQUEST );

    memcpy(packet.arp.senderMac, macAddress, sizeof(macAddress));
    memcpy(packet.arp.targetMac, dstAddress, sizeof(dstAddress));
    auto srcIp = deviceIp.ToInAddr().s_addr;
    memcpy(packet.arp.senderIp, &srcIp , sizeof(uint8_t) * 4);

    auto enumerator = network.GetEnumerator();
    while(enumerator.MoveNext())
    {
        auto currentAddress = enumerator.Current();
        auto dstIp = currentAddress.ToInAddr().s_addr;
        memcpy(packet.arp.targetIp, &dstIp, sizeof(dstIp));
        if (sendto(socket, &packet, sizeof(arpEthPacket), 0, (struct sockaddr *)&device, sizeof(struct sockaddr_ll)) <= 0)
        {
            Logger::Error("Arp Scanner", "Sending", errno);
        }
    }
    char buffer[256];

    fd_set readFileDescriptors;
    bzero(&readFileDescriptors, sizeof(readFileDescriptors));
    FD_SET(socket, &readFileDescriptors);
    while (true) {
        //Initialize timeout
        struct timeval timeout;
        bzero(&timeout, sizeof(timeout));
        timeout.tv_sec = this->arguments->maxRtt / 1000;
        timeout.tv_usec = this->arguments->maxRtt % 1000;

        //Select if incoming messages
        auto result = select(socket + 1, &readFileDescriptors, NULL, NULL, &timeout);
        if (result <= 0) {
            Logger::Debug("ARP Scan", "Select timeout");
            break;
        }
        int bufferSize = read(socket, buffer, sizeof(buffer));
        auto *arpHeader = (arpEthPacket *) (buffer);

        if (ntohs(arpHeader->arp.operationCode) == ARPOP_REPLY) {
            Logger::Debug("ARP Scan", "Reply");
            unsigned int intAddr;
            memcpy(&intAddr, arpHeader->arp.senderIp, 4);
            IpAddress addr(ntohl(intAddr));
            addressVector.push_back(addr);
        }
        else {
            continue;
        }
    }
    CloseSocket(socket);
    return addressVector;
}
