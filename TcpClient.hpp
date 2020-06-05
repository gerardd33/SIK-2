#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include "InputData.hpp"
#include <netdb.h>
#include <unistd.h>

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

	void establishTcpConnection();
	struct addrinfo* getAddressInfo();
	void setRadioTimeout();
};

#endif //TCPCLIENT_HPP_
