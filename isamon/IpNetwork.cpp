#include "IpNetwork.h"

#include <sstream>
#include "Logger.h"


IpNetwork::IpNetwork(): networkAddress(0), cidrMask(0)
{
}

IpNetwork::IpNetwork(unsigned int networkAddress, unsigned short cidrMask) : networkAddress(networkAddress), cidrMask(cidrMask)
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
	unsigned short cidrMask;
	int result = sscanf(cidr.c_str(), "%i.%i.%i.%i/%i", &firstOctet, &secondOctet, &thirdOctet, &fourthOctet, &cidrMask);
	if (result != 5) throw new std::exception();
	unsigned int ipAddress = (firstOctet << 24) | (secondOctet << 16) | (thirdOctet << 8) | fourthOctet;
	IpNetwork network(ipAddress, cidrMask);
	return network;
}
