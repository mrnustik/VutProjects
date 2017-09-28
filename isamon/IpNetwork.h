#pragma once
#include <string>


class IpNetwork
{
public:
	IpNetwork(const unsigned int networkAddress, const unsigned short cidrMask);
	~IpNetwork();
	std::string ToString() const;
	static IpNetwork FromCidr(std::string cidr);
private:
	const unsigned int networkAddress;
	const unsigned short cidrMask;
};

