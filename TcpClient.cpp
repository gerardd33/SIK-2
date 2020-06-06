#include "TcpClient.hpp"

TcpClient::TcpClient(InputData& inputData) : inputData(inputData), socketDescriptor(-1) {
	establishConnection();
	setRadioTimeout();
	this->socketFile = fdopen(this->socketDescriptor, "r+");
}

TcpClient::~TcpClient() {
	fclose(this->socketFile);
	close(this->socketDescriptor);
}

void TcpClient::establishConnection() {
	struct addrinfo* addressInfo = getAddressInfo();

	this->socketDescriptor = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
	if (socketDescriptor < 0) {
		ErrorHandler::syserr("socket");
	}

	if (connect(socketDescriptor, addressInfo->ai_addr, addressInfo->ai_addrlen) < 0) {
		ErrorHandler::syserr("connect");
	}

	freeaddrinfo(addressInfo);
}

addrinfo* TcpClient::getAddressInfo() {
	addrinfo addressHints{};
	addrinfo* addressResult;

	memset(&addressHints, 0, sizeof(struct addrinfo));
	addressHints.ai_family = AF_INET;
	addressHints.ai_socktype = SOCK_STREAM;
	addressHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(this->inputData.getRadioHost(), this->inputData.getRadioPort(),
		&addressHints, &addressResult) != 0) {
		ErrorHandler::fatal("Failed to find address");
	}

	return addressResult;
}

void TcpClient::setRadioTimeout() {
	struct timeval timeout{};
	timeout.tv_sec = this->inputData.getRadioTimeout();
	timeout.tv_usec = 0;

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}
}
