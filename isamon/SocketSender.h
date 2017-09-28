#pragma once
class SocketSender
{
public:
	SocketSender();
	~SocketSender();
protected:
	struct in_addr* GetMyIpAddress();
	unsigned short Checksum(void* buffer, int bufferSize);
};

