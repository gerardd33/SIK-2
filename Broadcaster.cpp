#include "Broadcaster.hpp"

sockaddr_in Broadcaster::getClientAddress(lastContactMapEntry& mapEntry) {
	return mapEntry.first;
}

long long Broadcaster::getLastContactTime(lastContactMapEntry& mapEntry) {
	return mapEntry.second;
}

bool Broadcaster::checkReceivedErrorType(ssize_t receivedLength) {
	if (receivedLength < 0) {
		if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
			return true; // continue;
		} else {
			ErrorHandler::syserr("recvfrom");
		}
	} else if (receivedLength == 0) {
		interrupt();
		return false; // return;
	}

	return true;
}

long long Broadcaster::getCurrentMiliseconds() {
	timeval currentTime{};
	gettimeofday(&currentTime, nullptr);
	long long currentMiliseconds = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
	return currentMiliseconds;
}

void Broadcaster::updateLastContact(sockaddr_in clientAddress) {
	this->lastContactMapMutex.lock();
	lastContactMap[clientAddress] = getCurrentMiliseconds();

	this->lastContactMapMutex.unlock();
}

void Broadcaster::handleClients() {
	struct sockaddr_in clientAddress{};
	char messageBuffer[MESSAGE_BUFFER_SIZE];

	while (!this->interrupted) {
		auto clientAddressLength = static_cast<socklen_t>(sizeof(clientAddress));
		ssize_t receivedLength = recvfrom(this->udpConnection.getSocketDescriptor(),
			messageBuffer, sizeof(messageBuffer),0,
			(struct sockaddr*) &clientAddress, &clientAddressLength);

		if (receivedLength <= 0) {
			if (checkReceivedErrorType(receivedLength)) {
				return;
			} else {
				continue;
			}
		}

		uint16_t messageType = ntohs(*((uint16_t*) messageBuffer));
		if (messageType == DISCOVER) {
			memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
			sendMessage(IAM, clientAddress, messageBuffer, this->radioName, strlen(this->radioName));
			updateLastContact(clientAddress);
		} else if (messageType == KEEPALIVE) {
			updateLastContact(clientAddress);
		} else {
			ErrorHandler::noexit("Invalid message type");
			continue;
		}
	}
}

Broadcaster::Broadcaster(InputData& inputData) : inputData(inputData), interrupted(false),
	udpConnection(inputData), radioName(UNKNOWN_RADIO_NAME) {
	// TODO multicast

	// TODO jesli bedzie dzialac, przesun do initialisation list
	this->clientHandler = std::thread(&Broadcaster::handleClients, this);
}

Broadcaster::~Broadcaster() {
	this->clientHandler.join();
}

std::vector<sockaddr_in> Broadcaster::getActiveClients() {
	lastContactMapMutex.lock();

	std::vector<sockaddr_in> clientsToRemove, activeClients;
	for (lastContactMapEntry& entry : lastContactMap) {
		sockaddr_in clientAddress = getClientAddress(entry);
		long long lastContactTime = getLastContactTime(entry);

		long long currentMiliseconds = getCurrentMiliseconds();
		if (currentMiliseconds - lastContactTime > inputData.getBroadcastTimeout())  {
			clientsToRemove.push_back(clientAddress);
		} else {
			activeClients.push_back(clientAddress);
		}
	}

	lastContactMapMutex.unlock();
	return activeClients;
}

void Broadcaster::sendMessage(uint16_t messageType, const sockaddr_in clientAddress, char* messageBuffer, const char* messageContent,
	size_t messageSize) {
	auto clientAddressLength = static_cast<socklen_t>(sizeof(clientAddress));

	*((uint16_t*) messageBuffer) = htons(messageType);
	*((uint16_t*) (messageBuffer + HEADER_FIELD_SIZE)) = htons(messageSize);
	strcpy(messageBuffer + 2 * HEADER_FIELD_SIZE, messageContent);

	size_t messageLength = 2 * HEADER_FIELD_SIZE + messageSize;
	ssize_t sentLength = sendto(this->udpConnection.getSocketDescriptor(), messageBuffer,
								messageLength, 0, (struct sockaddr*) &clientAddress, clientAddressLength);

	if (sentLength < 0) {
		ErrorHandler::syserr("sendto");
	}
}

void Broadcaster::broadcastAudio(const char* audioBuffer, size_t dataSize) {
	ErrorHandler::debug("broadcasting audio");
	std::vector<sockaddr_in> activeClients = getActiveClients();

	char messageBuffer[MESSAGE_BUFFER_SIZE];
	for (auto& clientAddress : activeClients) {
		memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
		sendMessage(AUDIO, clientAddress, messageBuffer, audioBuffer, dataSize);
	}
}

void Broadcaster::broadcastMetadata(const char* metadataBuffer, size_t dataSize) {
	ErrorHandler::debug("broadcasting metadata");
	std::vector<sockaddr_in> activeClients = getActiveClients();

	char messageBuffer[MESSAGE_BUFFER_SIZE];
	for (auto& clientAddress : activeClients) {
		memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
		sendMessage(METADATA, clientAddress, messageBuffer, metadataBuffer, dataSize);
	}
}
