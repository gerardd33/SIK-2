#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include <netdb.h>
#include <unistd.h>
#include <cstdio>

class UdpClient {
public:
	explicit UdpClient();
	~UdpClient();

	FILE* getSocketFile() {
		return socketFile;
	}

private:
	FILE* socketFile;
	int socketDescriptor;

	int establishTcpConnection();
	struct addrinfo* getAddressInfo();
	void setTimeout();
};

#endif //UDPCLIENT_HPP_
