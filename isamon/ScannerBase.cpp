#include "ScannerBase.h"
#include "Logger.h"
#include <unistd.h>
#include <exception>
#include <sys/socket.h>
#include <fcntl.h>

#define HOSTNAME_SIZE 256

ScannerBase::ScannerBase()
{
}


ScannerBase::~ScannerBase()
{
}

in_addr* ScannerBase::GetMyIpAddress()
{
	char hostname[HOSTNAME_SIZE];
	int result = gethostname(hostname, HOSTNAME_SIZE);
	if(result < 0)
	{
		//TODO change erxception
		throw new std::exception();
	}
	
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
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
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
	openSocketCount++;
	return sockNumber;
}



bool ScannerBase::CanOpenSocket() {
	return openSocketCount + 1 <= MaximumSocketsCount;
}

void ScannerBase::CloseSocket(int socketNumber) {
	close(socketNumber);
	openSocketCount--;
}

void ScannerBase::SetNonBlocking(int socketFd) {
    int flags = fcntl(socketFd, F_GETFL, 0);
    fcntl(socketFd, F_SETFL, flags | O_NONBLOCK);
}
