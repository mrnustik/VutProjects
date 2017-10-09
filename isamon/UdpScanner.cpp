#include "UdpScanner.h"
#include "Logger.h"
#include <cstring>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <netdb.h>

UdpScanner::UdpScanner(const Arguments* arguments): arguments(arguments)
{
}

UdpScanner::~UdpScanner()
{
}

bool UdpScanner::Scan(IpAddress& address, int port)
{

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket < 0)
	{
		Logger::Error("UdpScanner", "Could not create UDP socket");
		return false;
	}
	int icmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (icmpSocket < 0)
	{
		Logger::Error("UdpScanner", "Could not create ICMP socket");
		close(udpSocket);
		return false;
	}
	char buffer[128];
	bzero(buffer, sizeof(buffer));
	strcpy(buffer, "Hello");

	sockaddr_in servaddr;
	GetSocketAddress(address, &servaddr, port);
	bzero(&(servaddr.sin_zero), 8);
	auto size = (socklen_t) sizeof servaddr; 

	if (sendto(udpSocket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&servaddr, size) < 0)
	{
		std::cerr << strerror(errno) << std::endl;
		Logger::Error("UdpScanner", "Sending");
		close(icmpSocket);
		close(udpSocket);
		return false;
	}
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(icmpSocket, &fds);
	struct timeval tv;
	tv.tv_sec = arguments->maxRtt / 1000;             /* 10 second timeout */
	tv.tv_usec = arguments->maxRtt % 1000;
	bzero(buffer, sizeof buffer);
	if (select(icmpSocket + 1, &fds, NULL, NULL, &tv) > 0)
	{
		recvfrom(icmpSocket, &buffer, sizeof(buffer), 0x0, (struct sockaddr *)&servaddr, &size);
	}
	else if (!FD_ISSET(icmpSocket, &fds))
	{
		close(icmpSocket);
		close(udpSocket);
		return true;
	}
	else
	{
		close(icmpSocket);
		close(udpSocket);
		return false;
	}

	struct ip * ip_header = (struct ip *) buffer;
	const int ipLength = ip_header->ip_hl << 2;

	struct icmp * icmp_header = (struct icmp *)(buffer + ipLength);

	if ((icmp_header->icmp_type == ICMP_UNREACH) && (icmp_header->icmp_code == ICMP_UNREACH_PORT))
	{
		close(icmpSocket);
		close(udpSocket);
		return false;
	}
	close(icmpSocket);
	close(udpSocket);
	return true;
}

void UdpScanner::GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress, int port)
{
	socketAddress->sin_addr = address.ToInAddr();
	socketAddress->sin_family = AF_INET;
	socketAddress->sin_port = htons(port);
}

