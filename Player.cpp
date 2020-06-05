#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(inputData);
	FILE* radioSocketFile = tcpClient.getSocketFile();
	// TODO usun komentarz
	//FILE* radioSocketFile = fopen("request.txt", "r+");

	FILE* broadcastSocketFile = nullptr;
	if (inputData.isBroadcasting()) {
		UdpClient udpClient(inputData);
		broadcastSocketFile = udpClient.getSocketFile();
	}

	RequestSender requestSender(inputData, radioSocketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(inputData, radioSocketFile, broadcastSocketFile);
	responseProcessor.processServerResponse();
}

