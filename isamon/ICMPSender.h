#pragma once
#include "IpAddress.h"
#include "Arguments.h"
#include "SocketSender.h"

class ICMPSender : SocketSender
{
public:
	ICMPSender(const Arguments* arguments);
	~ICMPSender();
	bool SendPing(IpAddress& address);
private:
	void GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress);
	const Arguments* arguments;
protected:
	static const int IcmpEchoId = 1234;
};

