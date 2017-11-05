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
	char del1, del2, del3, del4;
	unsigned short cidrMask;
	std::stringstream stream(cidr);
	stream >> firstOctet >> del1 >> secondOctet >> del2 >> thirdOctet >> del3 >> fourthOctet >> del4 >> cidrMask;
	unsigned int ipAddress = ((firstOctet & 255) << 24) | ((secondOctet & 255) << 16) | ((thirdOctet & 255) << 8) | (fourthOctet & 255);
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

IpAddress IpNetwork::NetworkAddress() const {
	return IpAddress(networkAddress);
}


