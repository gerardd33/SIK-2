#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(inputData);

	RequestSender requestSender(inputData, tcpClient);
	requestSender.sendRequest();

	if (this->inputData.isBroadcasting()) { // Part B
		Broadcaster broadcaster(inputData);
		ResponseProcessor responseProcessor(inputData, tcpClient, &broadcaster);
		responseProcessor.processServerResponse();
	} else { // Part A only
		ResponseProcessor responseProcessor(inputData, tcpClient, nullptr);
		responseProcessor.processServerResponse();
	}
}

