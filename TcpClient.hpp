#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <netdb.h>
#include <unistd.h>

#include "InputData.hpp"

class TcpClient {
public:
	explicit TcpClient(InputData& inputData);
	~TcpClient();

	FILE* getSocketFile() {
		return socketFile;
	}

private:
	InputData& inputData;
	FILE* socketFile;
	int socketDescriptor;

	void establishConnection();
	addrinfo* getAddressInfo();
	void setRadioTimeout();
};

#endif  // TCPCLIENT_HPP_
