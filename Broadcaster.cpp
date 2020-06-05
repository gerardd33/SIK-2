#include "Broadcaster.hpp"

sockaddr_in Broadcaster::getClientAddress(std::pair<sockaddr_in, long long> mapEntry) {
	return mapEntry.first;
}

long long Broadcaster::getLastContactTime(std::pair<sockaddr_in, long long> mapEntry) {
	return mapEntry.second;
}

void Broadcaster::handleClients() {

}

Broadcaster::Broadcaster(InputData& inputData) : inputData(inputData), broadcastSocketFile(nullptr) {
	UdpClient udpClient(inputData);
	this->broadcastSocketFile = udpClient.getSocketFile();

}

Broadcaster::~Broadcaster() {
	// ...
}
