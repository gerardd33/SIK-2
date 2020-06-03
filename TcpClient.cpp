#include "TcpClient.hpp"

TcpClient::TcpClient(RadioInfo& radioInfo) : radioInfo(radioInfo) {
	int socketFd = establishTcpConnection();
	this->socketFile = fdopen(socketFd, "r+");
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
