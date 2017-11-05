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

void OnRecieve(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
	std::cout << user;
	std::cout << h->len;
	std::cout << bytes;
}


void UdpScanner::Scan(IpAddress &address) {
	int icmpSocket = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int udpSocket = OpenSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SetNonBlocking(udpSocket);
	char buffer[256];
	std::vector<int> ports;
	struct sockaddr_in socketAddress;
	bzero(&socketAddress, sizeof(socketAddress));
	for(int port = 1; port<= 65335; port++) {
		GetSocketAddress(address, port, &socketAddress);
		ports.push_back(port);
		sendto(udpSocket, "buffer", 5, 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));

		timeval timeout;
		bzero(&timeout, sizeof(timeout));
		timeout.tv_sec = arguments->maxRtt / 1000;
		timeout.tv_usec = arguments->maxRtt % 1000;

		fd_set readFileDescriptors;
		bzero(&readFileDescriptors, sizeof(readFileDescriptors));
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

