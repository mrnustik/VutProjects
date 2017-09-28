#include "Application.h"


Application::Application(const Arguments* arguments): arguments(arguments) 
{
}

int Application::Run()
{
	if(this->arguments->flagHelp)
	{
		this->PrintHelp();
		return 0;
	}
	return 0;
}

void Application::PrintHelp()
{
	std::cout << "isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask>" << std::endl;
	std::cout << "-h --help -- prints this help" << std::endl;
	std::cout << "-i --interface <interface> -- interface that will be used for scanning" << std::endl;
	std::cout << "-n --network <net_address/network> -- network IP address in CIDR format" << std::endl;
	std::cout << "-t --tcp -- scan for open TCP ports" << std::endl;
	std::cout << "-u --udp -- scan for open UDP ports" << std::endl;
	std::cout << "-p --port <port> -- opened port number to be scanned for" << std::endl;
	std::cout << "-w --wait <ms> -- maximum RTT";
}

Application::~Application()
{
}
