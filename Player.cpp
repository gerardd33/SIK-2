#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(inputData);
	FILE* radioSocketFile = tcpClient.getSocketFile();

	RequestSender requestSender(inputData, radioSocketFile);
	requestSender.sendRequest();

	if (this->inputData.isBroadcasting()) { // Part B
		Broadcaster broadcaster(inputData);
		ResponseProcessor responseProcessor(inputData, radioSocketFile, &broadcaster);
		responseProcessor.processServerResponse();
	} else { // Part A only
		ResponseProcessor responseProcessor(inputData, radioSocketFile, nullptr);
		responseProcessor.processServerResponse();
	}
}

