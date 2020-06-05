#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include "RadioInfo.hpp"
#include <netdb.h>
#include <unistd.h>

class TcpClient {
public:
	explicit TcpClient(RadioInfo& radioInfo);
	~TcpClient();

	FILE* getSocketFile() {
		return socketFile;
	}

private:
	RadioInfo& radioInfo;
	FILE* socketFile;
	int socketDescriptor;

	int establishTcpConnection();
	struct addrinfo* getAddressInfo();
	void setTimeout();
};

#endif //TCPCLIENT_HPP_
