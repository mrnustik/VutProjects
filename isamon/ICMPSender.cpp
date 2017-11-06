#include "ICMPSender.h"
#include <netinet/ip_icmp.h>
#include <cstring>
#include <thread>
#include <future>
#include <pcap.h>
#include "Logger.h"


IcmpScanner::IcmpScanner(const Arguments* arguments): arguments(arguments), ipVector(){
}


IcmpScanner::~IcmpScanner()
{
}


std::vector<IpAddress> IcmpScanner::ScanNetwork(IpNetwork network) {
    std::vector<IpAddress> ipVector;
    char pcapBuffer[PCAP_ERRBUF_SIZE];
    unsigned char buffer[1024];
    pcap_if_t* devices,* device;
    pcap_findalldevs(&devices, pcapBuffer);
    for(device = devices; device != nullptr; device = device->next) {
        if(strcmp(device->name, "any") == 0) continue;
        if(!arguments->interfaceName.empty() && arguments->interfaceName != device->name) continue;
        auto socket = this->OpenSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        if(!TryBindSocketToInterface(socket, device->name)) continue;

        auto enumerator = network.GetEnumerator();
        while (enumerator.MoveNext()) {
            //Send ICMP Echo to broadcast address
            auto address = enumerator.Current();
            SendIcmpEcho(socket, address);
            Logger::Debug("ICMP Scan", "ICMP send to address " + address.ToString());
        }

        fd_set readFileDescriptors{};
        FD_SET(socket, &readFileDescriptors);
        while (true) {
            //Initialize timeout
            struct timeval timeout{};
            timeout.tv_sec = this->arguments->maxRtt / 1000;
            timeout.tv_usec = this->arguments->maxRtt % 1000;

            //Select if incoming messages
            struct icmphdr receivedHeader{};
            auto result = select(socket + 1, &readFileDescriptors, NULL, NULL, &timeout);
            if (result <= 0) {
                Logger::Debug("ICMPScanner", "Select timeout");
                break;
            }

            //Receive incoming message
            result = recv(socket, buffer, sizeof buffer, 0);
            if (result <= 0) {
                continue;
            }
            if (result < sizeof receivedHeader) {
                continue;
            }

            //Read the ip header
            auto *ip_header = (ip *) buffer;
            const int ipLength = ip_header->ip_hl << 2;
            //Read the ICMP message
            memcpy(&receivedHeader, buffer + ipLength, sizeof receivedHeader);

            //Is valid ICMP echo reply?
            if (receivedHeader.type == ICMP_ECHOREPLY && receivedHeader.un.echo.id == IcmpEchoId) {
                if(!ContainsIpAddres(ipVector, ntohl(ip_header->ip_src.s_addr))) {
                    ipVector.emplace_back(ntohl(ip_header->ip_src.s_addr));

                    Logger::Debug("ICMPScanner", "Echo reply");
                }
            } else {
                Logger::Debug("ICMPScanner", "Invalid ICMP message");
            }
        }
        CloseSocket(socket);
    }
	return ipVector;
}

ssize_t IcmpScanner::SendIcmpEcho(int socket, IpAddress &toAddress) {
	char buffer[256];
	struct sockaddr_in address{};
	bzero(&address, sizeof(address));
	GetSocketAddress(toAddress, &address);

	//Create ICMP echo header
	struct icmphdr icmp_hdr{};
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.id = IcmpEchoId;
	icmp_hdr.un.echo.sequence = 1;
	icmp_hdr.checksum = Checksum(&icmp_hdr, sizeof icmp_hdr);
	memcpy(buffer, &icmp_hdr, sizeof icmp_hdr);

	//Send the request to broadcast
	auto result = sendto(socket, buffer, sizeof(icmp_hdr), 0, (struct sockaddr*) &address, sizeof(address));
	if(result < 0){
		int err = errno;
		Logger::Error("ICMP Scan", "Can't send ICMP Echo message", err);
		throw std::exception();
	}

	return result;
}

bool IcmpScanner::ContainsIpAddres(std::vector<IpAddress> vector, uint32_t address) {
    for (auto iterator = vector.begin(); iterator != vector.end(); ++iterator) {
        if(iterator->address == address) return true;
    }
    return false;
}
