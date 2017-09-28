#include "IpNetwork.h"

#include <sstream>


IpNetwork::IpNetwork(const unsigned int networkAddress, const unsigned short cidrMask) : networkAddress(networkAddress), cidrMask(cidrMask)
{
}

IpNetwork::~IpNetwork()
{
}

std::string IpNetwork::ToString() const
{
	short fourthOctet = this->networkAddress & 255;
	short thirdOctet = (this->networkAddress >> 8) & 255;
	short secondOctet = (this->networkAddress >> 16) & 255;
	short firstOctet = (this->networkAddress >> 24) & 255;
	std::stringstream stream;
	stream << firstOctet << ".";
	stream << secondOctet << ".";
	stream << thirdOctet << ".";
	stream << fourthOctet;
	stream << "/" << this->cidrMask;
	return stream.str();
}

IpNetwork IpNetwork::FromCidr(std::string cidr)
{
	short firstOctet;
	short secondOctet;
	short thirdOctet;
	short fourthOctet;
	const unsigned short cidrMask;
	int result = sscanf(cidr.c_str(), "%i.%i.%i.%i/%i", &firstOctet, &secondOctet, &thirdOctet, &fourthOctet, &cidrMask);
	if (result != 5) throw new std::exception();
	const unsigned int ipAddress = (firstOctet << 24) | (secondOctet << 16) | (thirdOctet << 8) | firstOctet;
	return IpNetwork(ipAddress, cidrMask);
}
