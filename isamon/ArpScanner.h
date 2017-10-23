#pragma once
#include "IpAddress.h"
#include "Arguments.h"
#include <pcap/pcap.h>

class ArpScanner
{
public:
	ArpScanner(const Arguments* arguments);
	~ArpScanner();
	
	bool ScanAddress(IpAddress&  address);
private:
	const Arguments* arguments;
	char ErrorBuffer[PCAP_ERRBUF_SIZE];
	bool found;
};

