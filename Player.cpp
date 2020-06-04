#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(radioInfo);
	FILE* socketFile = tcpClient.getSocketFile();

	RequestSender requestSender(radioInfo, socketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(radioInfo, socketFile);
	responseProcessor.processServerResponse();
}

