#include "TCPScanner.h"
#include "Logger.h"
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include "Arguments.h"
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <fcntl.h>

TcpScanner::TcpScanner(const Arguments* arguments): arguments(arguments) {
}


TcpScanner::~TcpScanner()
{
}

bool TcpScanner::Scan(IpAddress adress, int port)
{
	struct sockaddr_in serv_addr;
	long arg;
	struct timeval tv;

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
