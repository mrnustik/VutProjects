#include "ICMPSender.h"
#include "Arguments.h"
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

bool IcmpScanner::SendPing(IpAddress& address)
{
	struct icmphdr icmp_hdr;
	struct sockaddr_in addr;
	struct timeval timeout = { arguments->maxRtt / 1000 , arguments->maxRtt % 1000 }; //wait max 3 seconds for a reply
	memset(&icmp_hdr, 0, sizeof icmp_hdr);
	memset(&addr, 0, sizeof addr);
	int socketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socketFd < 0)
	{
		std::cout << strerror(errno) << std::endl;
		Logger::Error("ICMPScanner", "Could not open socket");
		throw new std::exception();
	}
	GetSocketAddress(address, &addr);
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.id = IcmpEchoId;

	unsigned char buffer[1024];
	int result = 0;
	fd_set read_set;
	socklen_t len;
	struct icmphdr recieved_header;
	icmp_hdr.un.echo.sequence = 1;
	icmp_hdr.checksum = this->Checksum(&icmp_hdr, sizeof icmp_hdr);
	memcpy(buffer, &icmp_hdr, sizeof icmp_hdr);
	result = sendto(socketFd, buffer, sizeof icmp_hdr,
		0, (struct sockaddr*)&addr, sizeof addr);
	if (result <= 0)
	{
		Logger::Debug("ICMPScanner", "Cant send");
		return false;
	}
	Logger::Debug("ICMPScanner", "ECHO sent");
	memset(&read_set, 0, sizeof read_set);
	FD_SET(socketFd, &read_set);

	//wait for a reply with a timeout
	result = select(socketFd + 1, &read_set, NULL, NULL, &timeout);
	if (result == 0) {
		Logger::Debug("ICMPScanner", "Select timeout");
		return false;
	}
	else if (result < 0) {
		Logger::Debug("ICMPScanner", "Select timeout");
		return false;
	}
	//usleep(arguments->maxRtt);
	//we don't care about the sender address in this example..
	len = 0;
	result = recvfrom(socketFd, buffer, sizeof buffer, 0, (struct sockaddr*)&addr, &len);
	if (result <= 0) {
		return false;
	}
	if (result < sizeof recieved_header) {
		return false;
	}
	struct ip *ip_header = (ip*)buffer;
	const int ipLength = ip_header->ip_hl << 2;

	memcpy(&recieved_header, buffer + ipLength, sizeof recieved_header);
	if (recieved_header.type == ICMP_ECHOREPLY && recieved_header.un.echo.id == IcmpEchoId) {
		//Logger::Debug("ICMPScanner", "Echo reply");
		return true;
	}
	else
	{
		//Logger::Debug("ICMPScanner", "Invalid ICMP messager");
	}
	return false;
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

        BindSocketToInterface(socket, device->name);

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
                ipVector.emplace_back(ntohl(ip_header->ip_src.s_addr));
                Logger::Debug("ICMPScanner", "Echo reply");
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
