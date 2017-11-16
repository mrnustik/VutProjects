#include <vector>
#include <cstring>
#include "TCPScanner.h"
#include "Logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <time.h>

ScannerBase::ScannerBase()
{
}


ScannerBase::~ScannerBase()
{
}

long ScannerBase::GetCurrentMilis()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

unsigned short ScannerBase::Checksum(void* buffer, int bufferSize)
{
	unsigned short *buf = (unsigned short*) buffer;
	unsigned int sum = 0;
	for(sum = 0; bufferSize > 1; bufferSize -=2)
	{
		sum += *buf++;
	}
	if(bufferSize == 1)
	{
		sum += *(unsigned char*)buf;
	}
    while (sum>>16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
	return (unsigned short) ~sum;
}

int ScannerBase::OpenSocket(int domain, int type, int protocol) {
	int sockNumber = socket(domain, type, protocol);
	if(sockNumber < 0)
	{
		int err = errno;
		Logger::Error("Socket", "Can't open socket", err);
		throw std::exception();
	}
    if(sockNumber > maximumSocketNumber)
        maximumSocketNumber = sockNumber;
	openSocketCount++;
	return sockNumber;
}

void ScannerBase::CloseSocket(int socketNumber) {
	close(socketNumber);
	openSocketCount--;
}

void ScannerBase::SetNonBlocking(int socketFd) {
    int flags = fcntl(socketFd, F_GETFL, 0);
    fcntl(socketFd, F_SETFL, flags | O_NONBLOCK);
}

void ScannerBase::GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress)
{
	socketAddress->sin_addr = address.ToInAddr();
	socketAddress->sin_family = AF_INET;
}

void ScannerBase::GetSocketAddress(IpAddress &address, int port, struct sockaddr_in *socketAddress) {
    socketAddress->sin_addr = address.ToInAddr();
    socketAddress->sin_family = AF_INET;
    socketAddress->sin_port = htons(static_cast<uint16_t>(port));
}

bool ScannerBase::TryBindSocketToInterface(int socket, std::string interfaceName) {
    struct ifreq ifRequest;
    memset(&ifRequest, 0, sizeof(ifRequest));
    memcpy(&ifRequest.ifr_name,interfaceName.c_str(), sizeof(ifRequest.ifr_name));
    if (setsockopt(socket, SOL_SOCKET, SO_BINDTODEVICE, (void*) &ifRequest, sizeof(ifRequest)) < 0) {
        Logger::Error("Socket", "Can't bind to device: " + interfaceName);
        return false;
    }
    return true;
}