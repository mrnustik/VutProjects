#pragma once
class ScannerBase
{
public:
	ScannerBase();
	~ScannerBase();
protected:
	bool CanOpenSocket();
	int OpenSocket(int domain, int type, int protocol);
	void SetNonBlocking(int socketFd);
	void CloseSocket(int socketNumber);
	struct in_addr* GetMyIpAddress();
	unsigned short Checksum(void* buffer, int bufferSize);
private:
	int openSocketCount = 0;
	static const int MaximumSocketsCount = 100;
};

