#pragma once

#include "IpAddress.h"
#include "Arguments.h"

class ScannerBase
{

public:
	ScannerBase();
	~ScannerBase();

    void BindSocketToInterface(int socket, std::string interfaceName);

protected:
	bool CanOpenSocket();
	int OpenSocket(int domain, int type, int protocol);
	void SetNonBlocking(int socketFd);
	void CloseSocket(int socketNumber);
    int GetMaximumSocketNumber();

	struct in_addr* GetMyIpAddress();
	unsigned short Checksum(void* buffer, int bufferSize);

    void GetSocketAddress(IpAddress& address, struct sockaddr_in * socketAddress);
    void GetSocketAddress(IpAddress& address, int port ,struct sockaddr_in * socketAddress);

private:
	int openSocketCount = 0;
    int maximumSocketNumber = 0;
	static const int MaximumSocketsCount = 100;
};

