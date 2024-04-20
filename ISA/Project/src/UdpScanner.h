#pragma once
#include "Arguments.h"
#include <pcap/pcap.h>
#include "ScannerBase.h"

class UdpScanner : ScannerBase
{
public:
	UdpScanner(const Arguments *arguments);
	~UdpScanner();
	void Scan(IpAddress &address);
private:
	const Arguments *arguments;
};

