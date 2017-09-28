#include "ICMPSender.h"
#include "Arguments.h"
#include <netinet/ip_icmp.h>
#include <cstring>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>


#define PACKETSIZE	64
struct ICMPPacket
{
	struct icmphdr hdr;
	char msg[PACKETSIZE - sizeof(struct icmphdr)];
};

ICMPSender::ICMPSender(const Arguments* arguments): arguments(arguments) {
}


ICMPSender::~ICMPSender()
{
}

bool ICMPSender::SendPing(IpAddress& address)
{
	int socketNumber;
	struct sockaddr_in socketAddress;
	struct ICMPPacket packet;
	struct sockaddr_in r_addr;
	struct timeval t;
	char recieveBuffer[1024];
	auto proto = getprotobyname("ICMP");
	t.tv_usec = arguments->maxRtt;
	fd_set socks;
	bzero(&socketAddress, sizeof(socketAddress));
	bzero(&packet, sizeof(packet));
	GetSocketAddress(address, &socketAddress);
	auto addressLength = sizeof(socketAddress.sin_addr);
	if((socketNumber = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0)
	{
		int err = errno;
		switch (errno)
		{
		case EACCES:
			break;
		case EINVAL:
			break;
		}
		std::cout << strerror(err);
		//TODO Add socket exception
		throw new std::exception();
	}
	packet.hdr.type = ICMP_ECHO;
	packet.hdr.un.echo.id = getpid();
	packet.msg[0] = 0;
	packet.hdr.un.echo.sequence = 1;
	packet.hdr.checksum = this->Checksum(&packet, sizeof(packet));
	FD_ZERO(&socks);
	FD_SET(socketNumber, &socks);
	sendto(socketNumber, &packet, sizeof(packet), 0, (struct sockaddr*)&socketAddress, sizeof(socketAddress));
	select(socketNumber + 1, &socks, nullptr, nullptr, &t);
	recvfrom(socketNumber, recieveBuffer, sizeof(recieveBuffer), 0, (struct sockaddr*) &socketAddress, (socklen_t *) &addressLength);

}

void ICMPSender::GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress)
{
	socketAddress->sin_addr = address.ToInAddr();
	socketAddress->sin_port = 0;
	socketAddress->sin_family = AF_INET;
}
