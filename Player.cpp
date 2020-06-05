#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(radioInfo);
	FILE* radioSocketFile = tcpClient.getSocketFile();

	/*
	UdpClient udpClient();
	FILE*
	*/

	RequestSender requestSender(radioInfo, radioSocketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(radioInfo, radioSocketFile);
	responseProcessor.processServerResponse();
}

