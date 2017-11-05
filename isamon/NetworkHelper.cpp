#include "Application.h"
#include "NetworkHelper.h"
#include "Logger.h"

#include <net/if.h>
#include <pcap.h>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <unistd.h>

#define MAC_SIZE        ( 6 * sizeof(uint8_t) )

unsigned int NetworkHelper::GetNetworkInterfaceNumber(std::string name)
{
	unsigned index = if_nametoindex(name.c_str());
	if(index == 0)
	{
		Logger::Error("Interface", "Specified interface not found");
		throw std::exception();
	}
	return index;
}

std::string NetworkHelper::GetConnectedAdapter(IpNetwork network) {
	pcap_if_t* devices,* device;
	char PcapErrorBuffer[PCAP_ERRBUF_SIZE];
	pcap_findalldevs(&devices, PcapErrorBuffer);
	for(device = devices; device != nullptr; device = device->next){
		
		for(pcap_addr_t *address = device->addresses; address != nullptr; address = address->next)
		{
            if(address->netmask == nullptr
                || address->addr == nullptr)
                continue;
			auto deviceAddr = ((struct sockaddr_in*)address->addr)->sin_addr;
			auto maskAddr = ((struct sockaddr_in*)address->netmask)->sin_addr;
			auto networkAddr = deviceAddr.s_addr & maskAddr.s_addr;
			if(network.NetworkAddress().ToInAddr().s_addr == networkAddr)
			{
				std::string deviceName(device->name);
				pcap_freealldevs(devices);
				return deviceName;
			}
		}
	}
	pcap_freealldevs(devices);
	return std::string();
}

void NetworkHelper::GetMacAddress(uint8_t *address, std::string iNetName){
    struct ifreq ifRequest;
	bzero(&ifRequest, sizeof(ifRequest));
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    memcpy(ifRequest.ifr_ifrn.ifrn_name, iNetName.c_str(), iNetName.size());
    if(ioctl(sock, SIOCGIFHWADDR, &ifRequest) < 0){
        Logger::Error("Network Helper", "Getting MAC address", errno);
    }
    memcpy(address, ifRequest.ifr_ifru.ifru_hwaddr.sa_data, MAC_SIZE);
    close(sock);
}

IpAddress NetworkHelper::GetDeviceAddress(std::string ifaceName) {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    ifreq ifRequest;
	bzero(&ifRequest, sizeof(ifRequest));
    strncpy(ifRequest.ifr_name , ifaceName.c_str() , IFNAMSIZ -1);
    if ( ioctl (sock, SIOCGIFADDR, &ifRequest) < 0 ) {
        Logger::Error("ioctl", "Get IP address", errno);
    }
    close(sock);
    return IpAddress(ntohl(((struct sockaddr_in *)&ifRequest.ifr_addr)->sin_addr.s_addr));
}
