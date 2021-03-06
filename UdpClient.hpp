#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include <libnet.h>
#include <netdb.h>
#include <unistd.h>

#include "InputData.hpp"

class UdpClient {
public:
	explicit UdpClient(InputData& inputData);
	~UdpClient();

	int getSocketDescriptor() {
		return socketDescriptor;
	}

private:
	const unsigned int DEFAULT_TIMEOUT = 1;

	InputData& inputData;

	int socketDescriptor;
	struct sockaddr_in serverAddress;

	void establishConnection();
	void bindSocket();
	void setTimeout();
	void setMulticast();
};

#endif  // UDPCLIENT_HPP_