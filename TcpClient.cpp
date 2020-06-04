#include "TcpClient.hpp"

TcpClient::TcpClient(RadioInfo& radioInfo) : radioInfo(radioInfo) {
	this->socketFd = establishTcpConnection();
	setTimeout();
	this->socketFile = fdopen(this->socketFd, "r+");
}

int TcpClient::establishTcpConnection() {
	struct addrinfo* addressInfo = getAddressInfo();

	int socketFd = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
	if (socketFd < 0) {
		ErrorHandler::syserr("socket");
	}

	if (connect(socketFd, addressInfo->ai_addr, addressInfo->ai_addrlen)  < 0) {
		ErrorHandler::syserr("connect");
	}

	freeaddrinfo(addressInfo);
	return socketFd;
}

struct addrinfo* TcpClient::getAddressInfo() {
	struct addrinfo addressHints;
	struct addrinfo* addressResult;

	memset(&addressHints, 0, sizeof(struct addrinfo));
	addressHints.ai_family = AF_INET;
	addressHints.ai_socktype = SOCK_STREAM;
	addressHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(this->radioInfo.getRadioHost(), this->radioInfo.getRadioPort(),
		&addressHints, &addressResult) != 0) {
		ErrorHandler::fatal("Failed to find address");
	}

	return addressResult;
}

TcpClient::~TcpClient() {
	fclose(socketFile);
	close(socketFd);
}

void TcpClient::setTimeout() {
	struct timeval timeout;
	timeout.tv_sec = this->radioInfo.getTimeout();
	timeout.tv_usec = 0;

	if (setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}

	if (setsockopt(this->socketFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}
}
