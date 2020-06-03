#include "TcpClient.hpp"

TcpClient::TcpClient(RadioInfo& radioInfo) {
	int socketFd = establishTcpConnection(radioInfo);
	this->socketFile = fdopen(socketFd, "r+");
}

int TcpClient::establishTcpConnection(RadioInfo& radioInfo) {
	struct addrinfo* addressInfo = getAddressInfoFromRaw(radioInfo.getRadioHost(), radioInfo.getRadioPort());
	return 0;
}

struct addrinfo* TcpClient::getAddressInfoFromRaw(const char* radioHost, const char* radioPort) {
	struct addrinfo addressHints;
	struct addrinfo* addressResult;

	memset(&addressHints, 0, sizeof(struct addrinfo));
	addressHints.ai_family = AF_INET;
	addressHints.ai_socktype = SOCK_STREAM;
	addressHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(radioHost, radioPort, &addressHints, &addressResult) != 0) {

	}

	return nullptr;
}
