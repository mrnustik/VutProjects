#pragma once
#include "IpAddress.h"
#include "SocketSender.h"
#include "Arguments.h"

class TcpScanner : SocketSender
{
public:
	TcpScanner();
	TcpScanner(const Arguments* arguments);
	~TcpScanner();
	bool Scan(IpAddress adress, int port);
	const Arguments* arguments;
};

