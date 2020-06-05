#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(inputData);
	FILE* radioSocketFile = tcpClient.getSocketFile();

	UdpClient udpClient(inputData);
	FILE* broadcastSocketFile = udpClient.getSocketFile();

	RequestSender requestSender(inputData, radioSocketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(inputData, radioSocketFile, broadcastSocketFile);
	responseProcessor.processServerResponse();
}

