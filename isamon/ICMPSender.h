#pragma once

#include <vector>
#include "IpAddress.h"
#include "Arguments.h"
#include "ScannerBase.h"

class IcmpScanner : ScannerBase
{
public:
	IcmpScanner(const Arguments* arguments);
	~IcmpScanner();

	std::vector<IpAddress> ScanNetwork(IpNetwork network);

private:
    const Arguments* arguments;
protected:
	static const int IcmpEchoId = 1234;
	ssize_t SendIcmpEcho(int socket, IpAddress &toAddress);
    std::vector<IpAddress> ipVector;
};

