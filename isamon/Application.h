#pragma once
#include "Arguments.h"

class Application
{
public:
	Application(const Arguments* arguments);
	~Application();
	int Run();
private:
	void PrintHelp();
	const Arguments* arguments;
};

