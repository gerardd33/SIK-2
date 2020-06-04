#include "Player.hpp"

void Player::run() {
	/*
		printf("Host: %s:\n", radioInfo.getRadioHost());
		printf("Resource: %s:\n", radioInfo.getRadioResourcePath());
		printf("Port: %s\n", radioInfo.getRadioPort());
		printf("Timeout: %d\n", radioInfo.getTimeout());
		printf("Request metadata?: %d\n", radioInfo.isRequestMetadata());
	 */

	TcpClient tcpClient(radioInfo);
	FILE* socketFile = tcpClient.getSocketFile();
}
