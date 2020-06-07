#include "LastContactStorage.hpp"

std::vector<sockaddr_in> LastContactStorage::getActiveClients() {
	this->storageMutex.lock();

	std::vector<sockaddr_in> clientsToRemove, activeClients;
	for (lastContactMapEntry& entry : lastContactMap) {
		sockaddr_in clientAddress = getClientAddress(entry);
		long long lastContactTime = getLastContactTime(entry);

		long long currentMilliseconds = getCurrentMilliseconds();
		if (currentMilliseconds - lastContactTime >
			1000 * inputData.getBroadcastTimeout()) {
			clientsToRemove.push_back(clientAddress);
		} else {
			activeClients.push_back(clientAddress);
		}
	}

	for (auto& client : clientsToRemove) {
		lastContactMap.erase(client);
	}

	this->storageMutex.unlock();
	return activeClients;
}

void LastContactStorage::updateLastContact(sockaddr_in clientAddress) {
	this->storageMutex.lock();
	lastContactMap[clientAddress] = getCurrentMilliseconds();
	this->storageMutex.unlock();
}

long long LastContactStorage::getCurrentMilliseconds() {
	timeval currentTime{};
	gettimeofday(&currentTime, nullptr);
	long long currentMilliseconds =
		currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
	return currentMilliseconds;
}
