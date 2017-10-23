#pragma once
#include "Arguments.h"
#include <pcap/pcap.h>
#include "SocketSender.h"

class UdpScanner : SocketSender
{
public:
	UdpScanner(const Arguments *arguments);
	~UdpScanner();
	void* CraftPacket(pcap_if_t* device, const IpAddress& address, int port);
	bool Scan(IpAddress &address, int port);
	void GetSocketAddress(IpAddress& address, sockaddr_in* socketAddress, int port);
private:
	const Arguments *arguments;
};

