#include "Application.h"
#include "NetworkHelper.h"

#include <net/if.h>


unsigned int NetworkHelper::GetNetworkInterfaceNumber(std::string name)
{
	unsigned index = if_nametoindex(name.c_str());
	if(index == 0)
	{
		//TODO: Změnit exceptionu
		throw new std::exception();
	}
	return index;
}

std::string NetworkHelper::GetConnectedAdapter(IpNetwork network) {
	return std::string();
}