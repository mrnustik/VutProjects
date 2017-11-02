#pragma once
#include "IpAddress.h"
#include "ScannerBase.h"
#include "Arguments.h"

class TcpScanner : ScannerBase
{
public:
	TcpScanner();
	TcpScanner(const Arguments* arguments);
	~TcpScanner();
	bool Scan(IpAddress adress, int port);
	const Arguments* arguments;
};

