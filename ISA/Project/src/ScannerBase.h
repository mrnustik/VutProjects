#pragma once

#include "IpAddress.h"
#include "Arguments.h"

class ScannerBase
{

public:
	ScannerBase();
    virtual ~ScannerBase();
protected:
	int OpenSocket(int domain, int type, int protocol);
	void SetNonBlocking(int socketFd);
	void CloseSocket(int socketNumber);
	bool TryBindSocketToInterface(int socket, std::string interfaceName);

	unsigned short Checksum(void* buffer, int bufferSize);
	long GetCurrentMilis();
    void GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress);
    void GetSocketAddress(IpAddress& address, int port ,struct sockaddr_in * socketAddress);

private:
	int openSocketCount = 0;
    int maximumSocketNumber = 0;
};

