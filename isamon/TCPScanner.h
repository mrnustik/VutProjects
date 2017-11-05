#pragma once

#include <netinet/tcp.h>
#include <pcap.h>
#include "IpAddress.h"
#include "ScannerBase.h"
#include "Arguments.h"

struct TCPPseudoHeader {
	unsigned int sourceAddress;
	unsigned int destinationAddress;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcpLength;

	struct tcphdr tcp;
} __attribute__((aligned(256)));

class TcpScanner : ScannerBase
{
public:
	TcpScanner(const Arguments* arguments);
	~TcpScanner();
	const Arguments* arguments;
    void Scan(IpAddress address);
    tcphdr CreateTcpHeader() const;
	TCPPseudoHeader CreatePseudoHeader(pcap_if *pIf, IpAddress address,  tcphdr tcphdr);
	unsigned int GetInAddrFromPcapDevice(pcap_if *device);
};

