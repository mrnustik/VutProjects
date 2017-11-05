#include "UdpScanner.h"
#include "Logger.h"
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstring>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <cstdio>
#include <vector>
#include <algorithm>
static char ErrorBuffer[PCAP_ERRBUF_SIZE];
const static int PacketSize = sizeof(iphdr) + sizeof(udphdr) + 5;


struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

UdpScanner::UdpScanner(const Arguments* arguments) : arguments(arguments)
{
}

UdpScanner::~UdpScanner()
{
}

void* UdpScanner::CraftPacket(pcap_if_t* device, const IpAddress& address, int port)
{
	char buffer[PacketSize];
	memset(buffer, 0, sizeof(buffer));
	char *data = buffer + sizeof(iphdr) + sizeof(udphdr);
	memcpy(data, "UDPAB", strlen("UDPAB"));
	bpf_u_int32 ip_raw;
	bpf_u_int32 subnet_mask_raw;
//	int result = pcap_lookupnet(device->name, &ip_raw, &subnet_mask_raw, ErrorBuffer);
//	if (result != 0)
//	{
//		Logger::Error("UDP Create packet", "Can't lookup device" + std::string(device->name));
//		return nullptr;
//	}
	iphdr* ipHeader = (iphdr*)buffer;
	ipHeader->ihl = 5;
	ipHeader->version = 4;
	ipHeader->tos = 0;
	ipHeader->tot_len = PacketSize;
	ipHeader->id = htons(54321);
	ipHeader->frag_off = 0x00;
	ipHeader->ttl = 0xff;
	ipHeader->protocol = IPPROTO_UDP;
	ipHeader->check = 0;
	ipHeader->saddr = ip_raw;
	auto addressString = address.ToString();
	ipHeader->daddr = inet_addr(addressString.c_str());
	ipHeader->check = Checksum(buffer, ipHeader->tot_len);

	udphdr* udpHeader = (udphdr*)buffer + sizeof(struct iphdr);
	udpHeader->source = htons(6666);
	udpHeader->dest = htons(8622);
	udpHeader->len = htons(8 + 5); //tcp header size

	pseudo_header psh;

	psh.source_address = ip_raw;
	psh.dest_address = inet_addr(addressString.c_str());
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));

	int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	char *pseudogram = (char*)malloc(psize);

	memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header), udpHeader, sizeof(struct udphdr) + strlen(data));

	udpHeader->check = Checksum((unsigned short*)pseudogram, psize);

	return buffer;
}

void OnRecieve(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	std::cout << user;
	std::cout << h->len;
	std::cout << bytes;
}


bool UdpScanner::Scan(IpAddress& address, int port)
{
	int udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in servaddr;
	//GetSocketAddress(address, &servaddr, port);
	connect(udpSocket, (struct sockaddr*)&servaddr, sizeof(servaddr));
	send(udpSocket, "buffer", 6, 0);
	close(udpSocket);
	return false;
}


void UdpScanner::Scan(IpAddress &address) {
	int icmpSocket = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int udpSocket = OpenSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SetNonBlocking(udpSocket);
	char buffer[256];
	std::vector<int> ports;
	struct sockaddr_in socketAddress = {};
	for(int port = 1; port<= 65335; port++) {
		GetSocketAddress(address, port, &socketAddress);
		ports.push_back(port);
		sendto(udpSocket, "buffer", 5, 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));

		timeval timeout;
		timeout.tv_sec = arguments->maxRtt / 1000;
		timeout.tv_usec = arguments->maxRtt % 1000;

		fd_set readFileDescriptors{};
		FD_SET(icmpSocket, &readFileDescriptors);

		while (true) {
			FD_ZERO(&readFileDescriptors);
			FD_SET(icmpSocket, &readFileDescriptors);

			if (select(icmpSocket + 1, &readFileDescriptors, NULL, NULL, &timeout) > 0) {
				recvfrom(icmpSocket, &buffer, sizeof(buffer), 0x0, NULL, NULL);
			} else if (!FD_ISSET(udpSocket, &readFileDescriptors)) {
				break;
			} else {
				Logger::Error("UDP Scanner", "recvfrom()", errno);
				break;
			}

			struct ip *iphdr = (struct ip *) buffer;
			unsigned int iplen = iphdr->ip_hl << 2;
			struct icmp *icmp = (struct icmp *) (buffer + iplen);

			if (icmp->icmp_type == ICMP_UNREACH) {
				Logger::Debug("UDP Scan", "Icmp unreachable");
				struct udphdr *udp = (struct udphdr *) (icmp + 8 + sizeof(iphdr));
				ports.erase(std::remove(ports.begin(), ports.end(), port), ports.end());
				break;
			} else {
				Logger::Debug("UDP Scanner", "Other ICMP message");
				continue;
			}

		}
	}
	for(std::vector<int>::iterator it = ports.begin(); it != ports.end(); ++it) {
		std::cout << address.ToString() << " UDP " << *it << std::endl;
	}
	CloseSocket(icmpSocket);
	CloseSocket(udpSocket);
}

