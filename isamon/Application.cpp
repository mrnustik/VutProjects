#include "Application.h"
#include "Logger.h"
#include "ICMPSender.h"
#include "TcpScanner.h"
#include "UdpScanner.h"


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
	if(this->arguments->network.networkAddress != 0)
	{
		IpNetworkEnumerator enumerator = arguments->network.GetEnumerator();
		ICMPSender pingSender(arguments);
		TcpScanner tcpScanner(arguments);
		UdpScanner udpScanner(arguments);
		while(enumerator.MoveNext())
		{
			auto currentAddress = enumerator.Current();
			Logger::Debug("Scanning", "Currently on: " + currentAddress.ToString());
			if (pingSender.SendPing(currentAddress)) {
				std::cout << currentAddress.ToString() << std::endl;
				for (int port = 0; port <= 65535; port++)
				{
					if (arguments->flagTcp) {
						if (tcpScanner.Scan(currentAddress, port))
						{
							Logger::Debug("TCP Port", "Open port on: " + currentAddress.ToString() + ":" + std::to_string(port));
							std::cout << currentAddress.ToString() << " TCP " << port << std::endl;
						}
					}
				}
			}
		}
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
