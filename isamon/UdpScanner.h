#pragma once
#include "Arguments.h"

class UdpScanner
{
public:
	UdpScanner(const Arguments *arguments);
	~UdpScanner();
private:
	const Arguments *arguments;
};

