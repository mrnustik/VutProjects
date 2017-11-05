#include <vector>
#include "Application.h"
#include "Logger.h"
#include "ICMPSender.h"
#include "TCPScanner.h"
#include "UdpScanner.h"
#include "ArpScanner.h"
#include "NetworkHelper.h"


Application::Application(const Arguments* arguments): arguments(arguments)
{
}

int Application::Run()
{
    try {
        if (this->arguments->flagHelp) {
            this->PrintHelp();
            return 0;
        }
        if (this->arguments->network.networkAddress != 0) {
            ArpScanner arpScanner(arguments);
            IcmpScanner pingSender(arguments);
            TcpScanner tcpScanner(arguments);
            UdpScanner udpScanner(arguments);
            std::vector<IpAddress> knownHosts;
            auto adapter = NetworkHelper::GetConnectedAdapter(arguments->network);
            if (adapter.empty()) {
                //not local use ICMP scanning
                knownHosts = pingSender.ScanNetwork(this->arguments->network);
            } else {
                Logger::Debug("Application", "Adapter " + adapter);
                //local use ARP scanning
                knownHosts = arpScanner.ScanNetwork(this->arguments->network, adapter);
            }
            Logger::Debug("Hosts", "Found: " + std::to_string(knownHosts.size()) + " hosts");
            for (auto iterator = knownHosts.begin(); iterator != knownHosts.end(); ++iterator) {
                auto address = *iterator;
                std::cout << address.ToString() << std::endl;
                if (arguments->flagTcp) {
                    tcpScanner.Scan(address);
                }
                if (arguments->flagUdp) {
                    udpScanner.Scan(address);
                }
            }
            knownHosts.clear();
        }
        return 0;
    } catch (...) {
        Logger::Error("Error", "An error occured");
        return 1;
    }
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

