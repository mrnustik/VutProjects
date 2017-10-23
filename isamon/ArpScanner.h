#pragma once
#include "IpAddress.h"
#include "Arguments.h"

class ArpScanner
{
public:
	ArpScanner(const Arguments* arguments);
	~ArpScanner();
	bool ScanAddress(IpAddress&  address);
private:
	const Arguments* arguments;
};

