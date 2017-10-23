#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class IpAddress
{
public:
	IpAddress(unsigned int address);
	~IpAddress();
	struct in_addr ToInAddr();
	std::string ToString() const;
private:
	unsigned int address;
};

