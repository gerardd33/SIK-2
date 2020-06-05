#include "UdpClient.hpp"

void UdpClient::bindSocket() {
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(DEFAULT_PORT_NUMBER);

	if (bind(this->socketDescriptor, (struct sockaddr*) &serverAddress,
			 (socklen_t) sizeof(serverAddress)) < 0) {
		ErrorHandler::syserr("bind");
	}
}

void UdpClient::setTimeout() {
	struct timeval timeout;
	timeout.tv_sec = this->radioInfo.getBroadcastTimeout();
	timeout.tv_usec = 0;

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}
}

void UdpClient::establishUdpConnection() {
	this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketDescriptor < 0) {
		ErrorHandler::syserr("socket");
	}

	bindSocket();
}

UdpClient::~UdpClient() {
	fclose(this->socketFile);
	close(this->socketDescriptor);
}

UdpClient::UdpClient(RadioInfo& radioInfo) : radioInfo(radioInfo) {
	establishUdpConnection();
	setTimeout();
	this->socketFile = fdopen(this->socketDescriptor, "r+");
}

