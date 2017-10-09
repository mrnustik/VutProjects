#pragma once
#include "Arguments.h"

class UdpScanner
{
public:
	UdpScanner(const Arguments *arguments);
	~UdpScanner();
	bool Scan(IpAddress &address, int port);
	void GetSocketAddress(IpAddress& address, sockaddr_in* socketAddress, int port);
private:
	const Arguments *arguments;
};

