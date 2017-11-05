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
	struct {	                //  OFFSET		Bajt
		uint8_t dst_mac[6];		//  0 -  5		+6
		uint8_t src_mac[6];		//  6 - 11		+6
		uint8_t type[2];		// 12 - 13		+2
	} eth;
	struct {					//  OFFSET		Bajt
		uint16_t htype;         // 14 - 15		+2
		uint16_t ptype;			// 16 - 17		+2
		uint8_t hlen;			// 18 - 18		+1
		uint8_t plen;			// 19 - 19		+1
		uint16_t opcode;		// 20 - 21		+2
		uint8_t sender_mac[6];	// 22 - 27		+6
		uint8_t sender_ip[4];	// 28 - 31		+4
		uint8_t target_mac[6];	// 32 - 37		+6
		uint8_t target_ip[4];	// 38 - 41		+4
	} arp;

	uint8_t padding[36];          // 44 - XX
} arpEthPacket;

std::vector<IpAddress> ArpScanner::ScanNetwork(IpNetwork network, std::string adapter) {
    std::vector<IpAddress> addressVector;
	uint8_t macAddress[6];
	NetworkHelper::GetMacAddress(macAddress, adapter);
	struct sockaddr_ll device;
    bzero(&device, sizeof(device));
    int socket = OpenSocket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL) );
    BindSocketToInterface(socket, adapter);
	device.sll_ifindex = if_nametoindex(adapter.c_str());
	memcpy(&device.sll_addr, macAddress, 6*sizeof(uint8_t));
    device.sll_family = AF_PACKET;
    device.sll_halen = 6;
    IpAddress deviceIp = NetworkHelper::GetDeviceAddress(adapter);

    arpEthPacket packet;
    bzero(&packet, sizeof(packet));
    uint8_t dstAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(packet.eth.dst_mac, dstAddress , sizeof(dstAddress));
    memcpy(packet.eth.src_mac, macAddress, sizeof(macAddress));
    packet.eth.type[0] = ETH_P_ARP / 256;
    packet.eth.type[1] = ETH_P_ARP % 256;

    packet.arp.htype  = htons( 1 );             // 1 == Ethernet
    packet.arp.ptype  = htons(ETH_P_IP);
    packet.arp.hlen   = sizeof(uint8_t) * 6;
    packet.arp.plen   = sizeof(uint8_t) * 4;
    packet.arp.opcode = htons( ARPOP_REQUEST );

    memcpy(packet.arp.sender_mac, macAddress, sizeof(macAddress));
    memcpy(packet.arp.target_mac, dstAddress, sizeof(dstAddress));
    auto srcIp = deviceIp.ToInAddr().s_addr;
    memcpy(packet.arp.sender_ip, &srcIp , sizeof(uint8_t) * 4);

    auto enumerator = network.GetEnumerator();
    while(enumerator.MoveNext())
    {
        auto currentAddress = enumerator.Current();
        auto dstIp = currentAddress.ToInAddr().s_addr;
        memcpy(packet.arp.target_ip, &dstIp, sizeof(dstIp));
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

        if (ntohs(arpHeader->arp.opcode) == ARPOP_REPLY) {
            Logger::Debug("ARP Scan", "Reply");
            unsigned int intAddr;
            memcpy(&intAddr, arpHeader->arp.sender_ip, 4);
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
