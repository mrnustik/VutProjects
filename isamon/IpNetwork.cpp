#include "IpNetwork.h"

#include <sstream>
#include "Logger.h"
#include <cmath>


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
	int firstOctet;
	int secondOctet;
	int thirdOctet;
	int fourthOctet;
	unsigned short cidrMask;
	int result = sscanf(cidr.c_str(), "%d.%d.%d.%d/%d", &firstOctet, &secondOctet, &thirdOctet, &fourthOctet, &cidrMask);
	if (result != 5) throw new std::exception();
	unsigned int ipAddress = (firstOctet << 24) | (secondOctet << 16) | (thirdOctet << 8) | fourthOctet;
	IpNetwork network(ipAddress, cidrMask);
	return network;
}

IpNetworkEnumerator IpNetwork::GetEnumerator() const
{
	return IpNetworkEnumerator(this->FirstAddress(), this->BroadcastAddress());
}

unsigned int IpNetwork::FirstAddress() const
{
	return networkAddress + 1;
}

unsigned int IpNetwork::BroadcastAddress() const
{
	unsigned int oneBits = std::pow(2.0, 32 - this->cidrMask) - 1;
	return networkAddress | oneBits;
}


