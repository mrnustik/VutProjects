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
	std::cout << "\t-h --help -- prints this help" << std::endl;
	std::cout << "\t-i --interface <interface> -- interface that will be used for scanning" << std::endl;
	std::cout << "\t-n --network <net_address/network> -- network IP address in CIDR format" << std::endl;
	std::cout << "\t-t --tcp -- scan for open TCP ports" << std::endl;
	std::cout << "\t-u --udp -- scan for open UDP ports" << std::endl;
	std::cout << "\t-p --port <port> -- opened port number to be scanned for" << std::endl;
	std::cout << "\t-w --wait <ms> -- maximum RTT" << std::endl;
}

Application::~Application()
{
}
