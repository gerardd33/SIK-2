#include "UdpClient.hpp"

UdpClient::UdpClient(InputData& inputData) : inputData(inputData) {
	establishConnection();
	setTimeout();
	setMulticast();
}

UdpClient::~UdpClient() {
	close(this->socketDescriptor);
}

void UdpClient::establishConnection() {
	this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketDescriptor < 0) {
		ErrorHandler::syserr("socket");
	}

	bindSocket();
}

void UdpClient::bindSocket() {
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serverAddress.sin_port = htons(inputData.getBroadcastPort());

	if (bind(this->socketDescriptor,
			 reinterpret_cast<struct sockaddr*>(&this->serverAddress),
			 static_cast<socklen_t>(sizeof(this->serverAddress))) < 0) {
		ErrorHandler::syserr("bind");
	}
}

void UdpClient::setTimeout() {
	struct timeval timeout{};
	timeout.tv_sec = DEFAULT_TIMEOUT;
	timeout.tv_usec = 0;

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &timeout,
				   sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}

	if (setsockopt(this->socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, &timeout,
				   sizeof(timeout)) < 0) {
		ErrorHandler::syserr("setsockopt");
	}
}

void UdpClient::setMulticast() {
	if (this->inputData.isMulticast()) {
		struct ip_mreq requirements{};
		requirements.imr_interface.s_addr = htonl(INADDR_ANY);

		if (inet_aton(this->inputData.getBroadcastMulticastAddress(),
					  &requirements.imr_multiaddr) == 0) {
			ErrorHandler::fatal("Invalid multicast address");
		}

		if (setsockopt(this->socketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP,
					   &requirements, sizeof(ip_mreq)) < 0) {
			ErrorHandler::syserr("setsockopt");
		}
	}
}
