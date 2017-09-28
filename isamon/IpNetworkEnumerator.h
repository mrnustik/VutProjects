#pragma once
#include "IpAddress.h"

class IpNetworkEnumerator
{
public:
	IpNetworkEnumerator(unsigned firstAddress, unsigned broadcastAddress);
	~IpNetworkEnumerator();
	bool MoveNext();
	IpAddress Current() const;
	void Reset();
private:
	unsigned int position;
	unsigned firstAddress;
	unsigned broadcastAddress;
};

