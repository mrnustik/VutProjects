#pragma once
#include <string>

class NetworkHelper
{
public:
	static unsigned int GetNetworkInterfaceNumber(std::string name);
	static void GetMacAddress(uint8_t *address, std::string iNetName);
	static std::string GetConnectedAdapter(IpNetwork network);
};


