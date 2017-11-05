#include "IpAddress.h"

#include <sstream>

IpAddress::IpAddress(unsigned int address) : address(address)
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

std::string IpAddress::ToString() const
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
