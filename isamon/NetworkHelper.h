#pragma once
#include <string>

class NetworkHelper
{
public:
	static unsigned int GetNetworkInterfaceNumber(std::string name);

	static std::string GetConnectedAdapter(IpNetwork network);
};


