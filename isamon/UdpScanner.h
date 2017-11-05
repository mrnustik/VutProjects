#pragma once
#include "Arguments.h"
#include <pcap/pcap.h>
#include "ScannerBase.h"

class UdpScanner : ScannerBase
{
public:
	UdpScanner(const Arguments *arguments);
	~UdpScanner();
	void* CraftPacket(pcap_if_t* device, const IpAddress& address, int port);
	bool Scan(IpAddress &address, int port);
	void Scan(IpAddress &address);
private:
	const Arguments *arguments;
};

