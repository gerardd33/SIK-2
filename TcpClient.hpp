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
	FILE* socketFile;

	int establishTcpConnection(RadioInfo& radioInfo);
	struct addrinfo* getAddressInfoFromRaw(const char* radioHost, const char* radioPort);
};

#endif //TCPCLIENT_HPP_
