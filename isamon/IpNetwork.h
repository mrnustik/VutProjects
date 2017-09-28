#pragma once
#include <string>


class IpNetwork
{
public:
	IpNetwork();
	IpNetwork(unsigned int networkAddress, unsigned short cidrMask);
	~IpNetwork();
	std::string ToString() const;
	static IpNetwork FromCidr(std::string cidr);
private:
	unsigned int networkAddress;
	unsigned short cidrMask;
};

