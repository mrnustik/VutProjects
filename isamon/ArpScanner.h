#pragma once
#include "IpAddress.h"
#include "Arguments.h"
#include "ScannerBase.h"
#include <pcap/pcap.h>
#include <vector>

class ArpScanner : ScannerBase
{
public:
	ArpScanner(const Arguments* arguments);
	~ArpScanner();
	
	bool ScanAddress(IpAddress&  address);

    std::vector<IpAddress> ScanNetwork(IpNetwork network, std::string adapter);

private:
	const Arguments* arguments;
	char ErrorBuffer[PCAP_ERRBUF_SIZE];
	bool found;
};

