#include "UdpClient.hpp"

struct addrinfo* UdpClient::getAddressInfo() {
	return nullptr;
}

void UdpClient::setTimeout() {
	/*
	struct timeval timeout;
	timeout.tv_sec = this->radioInfo.getTimeout();
	timeout.tv_usec = 0;

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}
	 */
}

int UdpClient::establishUdpConnection() {
	return 0;
}

UdpClient::~UdpClient() {
	fclose(this->socketFile);
	close(this->socketDescriptor);
}

UdpClient::UdpClient() {
	this->socketDescriptor = establishUdpConnection();
	setTimeout();
	this->socketFile = fdopen(this->socketDescriptor, "r+");
}

