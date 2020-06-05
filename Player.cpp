#include "Player.hpp"

void Player::run() {
	TcpClient tcpClient(radioInfo);
	FILE* radioSocketFile = tcpClient.getSocketFile();

	//UdpClient udpClient(radioInfo);
	//FILE* broadcastSocketFile = udpClient.getSocketFile();

	RequestSender requestSender(radioInfo, radioSocketFile);
	requestSender.sendRequest();

	ResponseProcessor responseProcessor(radioInfo, radioSocketFile);
	responseProcessor.processServerResponse();
}

