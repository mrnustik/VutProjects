#include "SocketSender.h"
#include <unistd.h>
#include <exception>

#define HOSTNAME_SIZE 256

SocketSender::SocketSender()
{
}


SocketSender::~SocketSender()
{
}

in_addr* SocketSender::GetMyIpAddress()
{
	char hostname[HOSTNAME_SIZE];
	int result = gethostname(hostname, HOSTNAME_SIZE);
	if(result < 0)
	{
		//TODO change erxception
		throw new std::exception();
	}
	
}

unsigned short SocketSender::Checksum(void* buffer, int bufferSize)
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
