#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include "InputData.hpp"
#include <netdb.h>
#include <unistd.h>

class UdpClient {
public:
	explicit UdpClient(InputData& inputData);
	~UdpClient();

	FILE* getSocketFile() {
		return socketFile;
	}

private:
	InputData& inputData;
	FILE* socketFile;
	int socketDescriptor;

	void establishUdpConnection();
	void bindSocket();
	void setTimeout();

	const unsigned int DEFAULT_PORT_NUMBER = 10001;
};

#endif //UDPCLIENT_HPP_
