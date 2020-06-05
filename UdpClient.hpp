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
	FILE* socketFile;
	int socketDescriptor;

	void establishUdpConnection();
	struct addrinfo* getAddressInfo();
	void setTimeout();
};

#endif //UDPCLIENT_HPP_
