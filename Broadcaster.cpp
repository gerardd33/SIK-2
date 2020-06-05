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

	this->clientHandler = std::thread(&Broadcaster::handleClients, this);
}

Broadcaster::~Broadcaster() {
	this->clientHandler.join();
}

void Broadcaster::broadcastAudio(const char* audioBuffer, size_t dataSize) {
	fprintf(stderr, "broadcasting audio\n");
}

void Broadcaster::broadcastMetadata(const char* metadataBuffer, size_t dataSize) {
	fprintf(stderr, "broadcasting metadata\n");
}
