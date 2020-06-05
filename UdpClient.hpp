#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include "InputData.hpp"
#include <netdb.h>
#include <unistd.h>

class UdpClient {
public:
	explicit UdpClient(InputData& inputData);
	~UdpClient();

	int getSocketDescriptor() {
		return socketDescriptor;
	}

private:
	InputData& inputData;
	int socketDescriptor;
	struct sockaddr_in serverAddress;

	void establishUdpConnection();
	void bindSocket();
	void setTimeout();

	const unsigned int DEFAULT_TIMEOUT = 1;
};

#endif //UDPCLIENT_HPP_