#include "ICMPSender.h"
#include "Arguments.h"
#include <netinet/ip_icmp.h>
#include <cstring>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include "Logger.h"


ICMPSender::ICMPSender(const Arguments* arguments): arguments(arguments) {
}


ICMPSender::~ICMPSender()
{
}

bool ICMPSender::SendPing(IpAddress& address)
{
	struct icmphdr icmp_hdr;
	struct sockaddr_in addr;
	struct timeval timeout = { 3, 0 }; //wait max 3 seconds for a reply
	memset(&icmp_hdr, 0, sizeof icmp_hdr);
	memset(&addr, 0, sizeof addr);
	int socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socketFd < 0)
	{
		std::cout << strerror(errno) << std::endl;
		Logger::Error("ICMPScanner", "Could not open socket");
		throw new std::exception();
	}
	GetSocketAddress(address, &addr);
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.id = IcmpEchoId;

	unsigned char buffer[1024];
	int result = 0;
	fd_set read_set;
	socklen_t len;
	struct icmphdr recieved_header;

	icmp_hdr.un.echo.sequence = 1;
	icmp_hdr.checksum = this->Checksum(&icmp_hdr, sizeof icmp_hdr);
	memcpy(buffer, &icmp_hdr, sizeof icmp_hdr);
	result = sendto(socketFd, buffer, sizeof icmp_hdr,
		0, (struct sockaddr*)&addr, sizeof addr);
	if (result <= 0)
	{
		Logger::Debug("ICMPScanner", "Cant send");
		return false;
	}
	Logger::Debug("ICMPScanner", "ECHO sent");
	memset(&read_set, 0, sizeof read_set);
	FD_SET(socketFd, &read_set);

	//wait for a reply with a timeout
	result = select(socketFd + 1, &read_set, NULL, NULL, &timeout);
	if (result == 0) {
		Logger::Debug("ICMPScanner", "Select timeout");
		return false;
	}
	else if (result < 0) {
		Logger::Debug("ICMPScanner", "Select timeout");
		return false;
	}

	//we don't care about the sender address in this example..
	len = 0;
	result = recvfrom(socketFd, buffer, sizeof buffer, 0, (struct sockaddr*)&addr, &len);
	if (result <= 0) {
		return false;
	}
	if (result < sizeof recieved_header) {
		return false;
	}
	memcpy(&recieved_header, buffer + (result - 8), sizeof recieved_header);
	if (recieved_header.type == ICMP_ECHOREPLY && recieved_header.un.echo.id == IcmpEchoId) {
		Logger::Debug("ICMPScanner", "Echo reply");
		return true;
	}
	else {
		Logger::Debug("ICMPScanner", "Invalid ICMP messager");
	}
	return false;
}

void ICMPSender::GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress)
{
	socketAddress->sin_addr = address.ToInAddr();
	socketAddress->sin_family = AF_INET;
}
