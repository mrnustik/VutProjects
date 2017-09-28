#include "IpAddress.h"

#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>

IpAddress::IpAddress() : address(0)
{
}


IpAddress::~IpAddress()
{
}

in_addr IpAddress::ToInAddr()
{
	struct in_addr addr;
	inet_aton(this->ToString().c_str(), &addr);
	return addr;
}

std::string IpAddress::ToString()
{
	short fourthOctet = this->address & 255;
	short thirdOctet = (this->address >> 8) & 255;
	short secondOctet = (this->address >> 16) & 255;
	short firstOctet = (this->address >> 24) & 255;
	std::stringstream stream;
	stream << firstOctet << ".";
	stream << secondOctet << ".";
	stream << thirdOctet << ".";
	stream << fourthOctet;
	return stream.str();
}
