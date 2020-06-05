#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(inputData);
	FILE* radioSocketFile = tcpClient.getSocketFile();



	RequestSender requestSender(inputData, radioSocketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(inputData, radioSocketFile);
	responseProcessor.processServerResponse();
}

