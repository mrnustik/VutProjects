#include "IpNetworkEnumerator.h"

IpNetworkEnumerator::IpNetworkEnumerator(unsigned int firstAddress, unsigned int broadcastAddress) : position(-1), firstAddress(firstAddress), broadcastAddress(broadcastAddress)
{
}

IpNetworkEnumerator::~IpNetworkEnumerator()
{
}

bool IpNetworkEnumerator::MoveNext()
{
	unsigned int currentAddress = firstAddress + this->position;
	if(currentAddress + 1 >= broadcastAddress)
	{
		return false;
	}
	else
	{
		this->position++;
		return true;
	}
}

IpAddress IpNetworkEnumerator::Current() const
{
	return IpAddress(firstAddress + this->position);
}

void IpNetworkEnumerator::Reset()
{
	this->position = -1;
}
