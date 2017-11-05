#include "TCPScanner.h"
#include "Logger.h"
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include "Arguments.h"
#include "ScannerBase.h"
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <sys/timeb.h>
#include <pcap.h>
#include <net/if.h>

TcpScanner::TcpScanner(const Arguments* arguments): arguments(arguments) {
}


TcpScanner::~TcpScanner()
{
}

void TcpScanner::Scan(IpAddress address)
{
    char pcapBuffer[PCAP_ERRBUF_SIZE];
    pcap_if_t* devices,* device;
    pcap_findalldevs(&devices, pcapBuffer);
    for(device = devices; device != nullptr; device = device->next) {
        if(strcmp(device->name, "any") == 0) continue;
        if(!arguments->interfaceName.empty() && arguments->interfaceName != device->name) continue;

        Logger::Debug("TCP Scanner", "Send to device " + std::string(device->name));

        int socket = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        BindSocketToInterface(socket, arguments->interfaceName);

        char buffer[256];
        tcphdr tcph = CreateTcpHeader();

        SetNonBlocking(socket);

        iphdr iph;
        iph.ihl = 5;
        iph.version = 4;
        iph.tos = 0;
        iph.tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
        iph.id = htons (54321); //Id of this packet
        iph.frag_off = htons(16384);
        iph.ttl = 64;
        iph.protocol = IPPROTO_TCP;
        iph.check = 0;      //Set to 0 before calculating checksum
        iph.saddr = GetInAddrFromPcapDevice(device);
        iph.daddr = address.ToInAddr().s_addr;

        //IP_HDRINCL to tell the kernel that headers are included in the packet
        int one = 1;
        const int *val = &one;

        if (setsockopt (socket, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
        {
            printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
            exit(0);
        }
        short sourceportOffset = 50000;
        std::vector<int> openSockets;
        for (uint16_t port = 1; port <= 100; port++) {
            tcph.source = htons ( sourceportOffset + port % 100);
            tcph.dest = htons(port);
            TCPPseudoHeader pseudoHeader = CreatePseudoHeader(device,address, tcph);
            tcph.check = Checksum(&pseudoHeader, sizeof(TCPPseudoHeader));
            struct sockaddr_in socketAddress = {};
            GetSocketAddress(address, port, &socketAddress);
            memcpy(buffer, &iph, sizeof(iph));
            memcpy(buffer + sizeof(iph), &tcph, sizeof(tcph));
            sendto(socket, buffer, sizeof(tcph) + sizeof(iph), 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
            tcph.check = 0;
        }

        fd_set readFileDescriptors{};
        FD_SET(socket, &readFileDescriptors);
        while (true) {
            //Initialize timeout
            struct timeval timeout{};
            timeout.tv_sec = this->arguments->maxRtt / 1000;
            timeout.tv_usec = this->arguments->maxRtt % 1000;

            //Select if incoming messages
            struct tcphdr receivedHeader{};
            auto result = select(socket + 1, &readFileDescriptors, NULL, NULL, &timeout);
            if (result <= 0) {
                Logger::Debug("TCP Scanner", "Select timeout");
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
            //Read the TCP message
            memcpy(&receivedHeader, buffer + ipLength, sizeof receivedHeader);

            if (receivedHeader.ack == 1 && receivedHeader.syn == 1) {
                std::cout << address.ToString() << " TCP " <<  ntohs(receivedHeader.source) << std::endl;
            };
        }
        CloseSocket(socket);
    }
}

tcphdr TcpScanner::CreateTcpHeader() const {
    tcphdr tcph{};
    bzero(&tcph, sizeof(tcph));
    //TCP Header
    tcph.source = htons ( 54826);
    tcph.seq = htonl(1105024978);
    tcph.ack_seq = 0;
    tcph.doff = sizeof(struct tcphdr) / 4;      //Size of tcp header
    tcph.fin=0;
    tcph.syn=1;
    tcph.rst=0;
    tcph.psh=0;
    tcph.ack=0;
    tcph.urg=0;
    tcph.window = htons ( 14600 );  // maximum allowed window size
    tcph.check = 0; //if you set a checksum to zero, your kernel's IP stack should fill in the correct checksum during transmission
    tcph.urg_ptr = 0;
    return tcph;
}

bool TcpScanner::Scan(IpAddress adress, int port)
{
	struct sockaddr_in serv_addr;
	long arg;


	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		int err = errno;
		Logger::Debug("TCP scan", "Can't open socket " + std::to_string(err));
		//FIXME
		throw new std::exception();
	}
	struct hostent *server;
	if ((server = gethostbyname(adress.ToString().c_str())) == nullptr) {
		exit(EXIT_FAILURE);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(port);
	// Set non-blocking 
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(sockfd, &fdset);
    timeval tv;
	tv.tv_sec = arguments->maxRtt / 1000;             /* 10 second timeout */
	tv.tv_usec = arguments->maxRtt % 1000;
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)	{
		int err = errno;
		if(err == EINPROGRESS)
		{
			int res = select(sockfd + 1, nullptr, &fdset, nullptr, &tv);
			if(res > 0)
			{
				int valopt;
				socklen_t lon = sizeof(int);
				getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
				if (valopt) {
					close(sockfd);
					return false;
				}
				close(sockfd);
				return true;
			}
		}
		close(sockfd);
		return false;
	}
	close(sockfd);
	return true;
}

TCPPseudoHeader TcpScanner::CreatePseudoHeader(pcap_if *pIf, IpAddress address,  tcphdr tcpHeader) {
    TCPPseudoHeader pseudoHeader = {};
    bzero(&pseudoHeader, sizeof(TCPPseudoHeader));
    pseudoHeader.sourceAddress = GetInAddrFromPcapDevice(pIf);
    pseudoHeader.destinationAddress = address.ToInAddr().s_addr;
    pseudoHeader.placeholder = 0;
    pseudoHeader.protocol = IPPROTO_TCP;
    pseudoHeader.tcpLength = htons(sizeof(tcpHeader));
    memcpy(&pseudoHeader.tcp, &tcpHeader, sizeof(tcpHeader));
    return  pseudoHeader;
}

unsigned int TcpScanner::GetInAddrFromPcapDevice(pcap_if *device) {
    for(pcap_addr_t *a=device->addresses; a!=NULL; a=a->next) {
        if(a->addr->sa_family == AF_INET)
        {
            return ((struct sockaddr_in*) a->addr)->sin_addr.s_addr;
        }
    }
    return 0;
}

long TcpScanner::GetCurrentMilis() {
    timeval time ={};
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

