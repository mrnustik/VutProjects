#pragma once
#include "Arguments.h"

class Application
{
public:
	Application(const Arguments* arguments);
	int Run();
private:
	void PrintHelp();
	const Arguments* arguments;
	~Application();
};

