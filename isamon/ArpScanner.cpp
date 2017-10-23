#include "ArpScanner.h"

#include <ifaddrs.h>
#include "Logger.h"
#include <cstring>

#define ETH_ALEN	6		//Octets in one ethernet address
#define ETH_HLEN	14		//Total octets in heater
#define	ETH_FRAME_LEN	1514		//Max. octets in fram sans FCS


struct arp_hdr {
	unsigned short 	hrd;		        //hardware address type
	unsigned short 	proto;			//protocol adress type
	unsigned char 	hrd_add_len;		//hardware address length
	unsigned char 	proto_add_len;		//Protokoll adress length
	unsigned short 	op;			//Operation
	unsigned char 	smac[ETH_ALEN];		//source MAC (Ethernet Address)
	unsigned char 	sip[4];			//source IP
	unsigned char 	dmac[ETH_ALEN];		//destination MAC (Ethernet Address)
	unsigned char 	dip[4];			//destination IP 
	char 		pad[18];		//Padding, ARP-Requests are quite small (<64)
};

ArpScanner::ArpScanner(const Arguments* arguments):arguments(arguments)
{
}

ArpScanner::~ArpScanner()
{
}

bool ArpScanner::ScanAddress(IpAddress& address)
{
	int socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if(socketFd < 0)
	{
		int err = errno;
		Logger::Error("ArpScanner", "Can't create socket");
		Logger::Error("ArpScanner", std::string(strerror(err)));
		return false;
	}
	//struct ifaddrs ifa* = nullptr;

}
