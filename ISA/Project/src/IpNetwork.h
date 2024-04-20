#pragma once
#include <string>
#include "IpNetworkEnumerator.h"

class IpNetwork
{
public:
	IpNetwork();
	IpNetwork(unsigned int networkAddress, unsigned short cidrMask);
	~IpNetwork();
	std::string ToString() const;
	static IpNetwork FromCidr(std::string cidr);
	IpNetworkEnumerator GetEnumerator() const;
	unsigned int networkAddress;
	unsigned short cidrMask;
	IpAddress NetworkAddress() const;
	unsigned int FirstAddress() const;
	unsigned int BroadcastAddress() const;

};

