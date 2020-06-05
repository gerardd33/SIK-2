#include "UdpClient.hpp"

void UdpClient::bindSocket() {
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serverAddress.sin_port = htons(inputData.getBroadcastPort());

	if (bind(this->socketDescriptor, (struct sockaddr*) &this->serverAddress,
			 (socklen_t) sizeof(this->serverAddress)) < 0) {
		ErrorHandler::syserr("bind");
	}
}

void UdpClient::setTimeout() {
	struct timeval timeout;
	timeout.tv_sec = DEFAULT_TIMEOUT;
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
	close(this->socketDescriptor);
}

UdpClient::UdpClient(InputData& inputData) : inputData(inputData) {
	establishUdpConnection();
	setTimeout();
}
