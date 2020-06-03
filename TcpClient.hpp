#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include "RadioInfo.hpp"
#include <netdb.h>

class TcpClient {
public:
	explicit TcpClient(RadioInfo& radioInfo);

	FILE* getSocketFile() {
		return socketFile;
	}

private:
	RadioInfo& radioInfo;
	FILE* socketFile;

	int establishTcpConnection();
	struct addrinfo* getAddressInfo();
};

#endif //TCPCLIENT_HPP_
