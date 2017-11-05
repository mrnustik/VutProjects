#include "ArpScanner.h"

#include <ifaddrs.h>
#include "Logger.h"
#include "NetworkHelper.h"
#include <cstring>
#include <sys/ioctl.h> 
#include <pcap.h>
#include <net/if.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netpacket/packet.h>   // struct sockaddr_ll
#include <net/ethernet.h>       // ETHER_ADD_LEN, ETH_P_*
#include <netinet/if_ether.h>   // struct ether_arp
#include <linux/if_ether.h>     // ETH_P_ARP = 0x0806
#include <unistd.h>



ArpScanner::ArpScanner(const Arguments* arguments):arguments(arguments)
{
}

ArpScanner::~ArpScanner()
{
}

static bool found = false;

//void pcapCallback(u_char *user, const struct pcap_pkthdr *header,const u_char *packet)
//{
//	struct ether_arp *ether_arp;
//	u_char eth_str[19];
//	u_char ip_str[17];
//	uint32_t ip;
//
//	packet_t *arp_packet = (packet_t *)(packet);
//
//	Logger::Debug("Arp scan", "Pcap receievd");
//
//	/* We are only interested ARP replies. */
//	if (arp_packet->arp.opcode != ntohs(2))
//		return;
//
//	found = true;
//}

bool ArpScanner::ScanAddress(IpAddress& address)
{
//	uint8_t my_ethaddr[6];
//	char filter[1024];
//	struct bpf_program bpf;
//	int socketFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
//	struct ifreq ifr = { 0 };
//	if(socketFd < 0)
//	{
//		int err = errno;
//		Logger::Error("ArpScanner", "Can't create socket");
//		Logger::Error("ArpScanner", std::string(strerror(err)));
//		return false;
//	}
//	std::string ifaceName;
//	if(arguments->interfaceName.empty())
//	{
//		ifaceName = std::string(pcap_lookupdev(ErrorBuffer));
//	}
//	else
//	{
//		ifaceName = arguments->interfaceName;
//	}
//	Logger::Debug("Arp scan", "Getting MAC...");
//	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifaceName.c_str());
//	if(ioctl(socketFd, SIOCGIFHWADDR, &ifr) < 0)
//	{
//		Logger::Error("Arp scan", "Cant find mac address");
//		return false;
//	}
//
//	memcpy(my_ethaddr, ifr.ifr_hwaddr.sa_data, sizeof(my_ethaddr));
//
//	Logger::Debug("Arp scan", "Getting IP...");
//	strncpy(ifr.ifr_name, ifaceName.c_str(), IFNAMSIZ - 1);
//	if (ioctl(socketFd, SIOCGIFADDR, &ifr) < 0)
//	{
//		Logger::Error("Arp scan", "Can't get IP");
//		return false;
//	}
//	uint32_t my_ipaddr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
//
//	Logger::Debug("Arp scan", "Starting PCAP");
//	pcap_t *pcap = pcap_open_live(ifaceName.c_str(), 64, 1, 1, ErrorBuffer);
//	if (pcap == NULL) {
//		return false;
//	}
//	snprintf(filter, sizeof(filter),
//		"arp and ether dst %02x:%02x:%02x:%02x:%02x:%02x",
//		my_ethaddr[0], my_ethaddr[1], my_ethaddr[2],
//		my_ethaddr[3], my_ethaddr[4], my_ethaddr[5]);
//	if (pcap_compile(pcap, &bpf, filter, 1, 0) < 0)
//	{
//		pcap_close(pcap);
//		Logger::Error("Arp scan", "failed to compile pcap filter");
//		return false;
//	}
//	if (pcap_setfilter(pcap, &bpf) < 0)
//	{
//		pcap_close(pcap);
//		Logger::Error("Arp scan", "failed to set pcap filter");
//		return  false;
//	}
//
//	if (pcap_setnonblock(pcap, 1, ErrorBuffer) < 0)
//	{
//		pcap_close(pcap);
//		Logger::Error("Arp scan", "failed to set pcap to non blocking");
//	}
//
//	packet_t packet;
//	memset(packet.eth.dst_mac, 0xff, sizeof(packet.eth.dst_mac));
//	memcpy(packet.eth.src_mac, &my_ethaddr, sizeof(my_ethaddr));
//	packet.eth.type[0] = ETH_P_ARP / 256;
//	packet.eth.type[1] = ETH_P_ARP % 256;
//
//	packet.arp.htype = htons(1);             // 1 == Ethernet
//	packet.arp.ptype = htons(ETH_P_IP);
//	packet.arp.hlen = MAC_SIZE;
//	packet.arp.plen = IPV4_SIZE;
//	packet.arp.opcode = htons(1);
//	memcpy(packet.arp.sender_mac, &my_ethaddr, sizeof(my_ethaddr));
//	memcpy(packet.arp.sender_ip, &my_ipaddr, sizeof(my_ipaddr));
//	memset(packet.arp.target_mac, 0x00, MAC_SIZE);
//	inet_pton(AF_INET, address.ToString().c_str(), packet.arp.target_ip);
//
//	struct sockaddr_ll device;
//	if ((device.sll_ifindex = if_nametoindex(ifaceName.c_str())) == 0) {
//		Logger::Error("Arp scan", "Getting iface_number");
//		return false;
//	}
//	struct sockaddr_ll socket_address;
//	socket_address.sll_family = AF_PACKET;
//	socket_address.sll_protocol = htons(ETH_P_ARP);
//	socket_address.sll_pkttype = (PACKET_BROADCAST);
//	socket_address.sll_halen = MAC_SIZE;
//	socket_address.sll_ifindex = if_nametoindex(ifaceName.c_str());
//	socket_address.sll_addr[6] = 0x00;
//	socket_address.sll_addr[7] = 0x00;
//	memcpy(device.sll_addr, my_ethaddr, MAC_SIZE);
//	device.sll_family = AF_PACKET;
//	device.sll_halen = 6;
//	found = false;
//	auto result = sendto(socketFd, &packet, sizeof(packet), 0, (struct sockaddr*)&socket_address, sizeof(device));
//	if(result < 0)
//	{
//		perror("sendto()");
//		Logger::Error("Arp scan", "failed sending");
//
//	}
//	for (int i = 0; i < arguments->maxRtt/1000; i++) {
//		pcap_dispatch(pcap, -1, pcapCallback, NULL);
//		sleep(i);
//	}
//
//	close(socketFd);
//	pcap_close(pcap);

	return false;
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
} packet_t;

std::vector<IpAddress> ArpScanner::ScanNetwork(IpNetwork network, std::string adapter) {
	uint8_t macAddress[6];
	NetworkHelper::GetMacAddress(macAddress, adapter);
	struct sockaddr_ll device = { 0 };
	device.sll_ifindex = if_nametoindex(adapter.c_str());
	memcpy(&device.sll_addr, macAddress, 6*sizeof(uint8_t));

}
