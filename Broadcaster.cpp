#include "Broadcaster.hpp"

Broadcaster::Broadcaster(InputData& inputData)
	: inputData(inputData),
	  udpConnection(inputData),
	  radioName(UNKNOWN_RADIO_NAME),
	  lastContactStorage(inputData) {
	this->waitForRadioMutex.lock();
	this->clientHandler = std::thread(&Broadcaster::handleClients, this);
}

Broadcaster::~Broadcaster() {
	this->clientHandler.join();
	if (strcmp(this->radioName, UNKNOWN_RADIO_NAME) != 0) {
		free((char*) this->radioName);
	}
}

void Broadcaster::setRadioName(const char* newName) {
	this->radioName = strdup(newName);
	this->waitForRadioMutex.unlock();
}

void Broadcaster::handleClients() {
	struct sockaddr_in clientAddress{};
	char messageBuffer[MESSAGE_BUFFER_SIZE];

	this->waitForRadioMutex.lock();
	while (!Environment::interrupted) {
		auto clientAddressLength = static_cast<socklen_t>(sizeof(clientAddress));
		ssize_t receivedLength =
			recvfrom(this->udpConnection.getSocketDescriptor(), messageBuffer,
					 sizeof(messageBuffer), 0, reinterpret_cast<struct sockaddr*>(&clientAddress),
					 &clientAddressLength);

		if (receivedLength <= 0) {
			if (checkReceivedErrorType(receivedLength)) {
				continue;
			} else {
				return;
			}
		}

		uint16_t messageType = ntohs(*(reinterpret_cast<uint16_t*>(messageBuffer)));
		if (messageType == DISCOVER) {
			memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
			sendMessage(IAM, clientAddress, messageBuffer, this->radioName,
						strlen(this->radioName));
			this->lastContactStorage.updateLastContact(clientAddress);
		} else if (messageType == KEEPALIVE) {
			this->lastContactStorage.updateLastContact(clientAddress);
		}
	}
}

void Broadcaster::sendMessage(uint16_t messageType, sockaddr_in& clientAddress,
							  char* messageBuffer, const char* messageContent,
							  size_t contentSize) {
	auto clientAddressLength = static_cast<socklen_t>(sizeof(clientAddress));

	*(reinterpret_cast<uint16_t*>(messageBuffer)) = htons(messageType);
	*(reinterpret_cast<uint16_t*>((messageBuffer + HEADER_FIELD_SIZE))) = htons(contentSize);
	copyContentToBuffer(messageBuffer + 2 * HEADER_FIELD_SIZE, messageContent,
						contentSize);

	size_t messageLength = 2 * HEADER_FIELD_SIZE + contentSize;
	ssize_t sentLength = sendto(
		this->udpConnection.getSocketDescriptor(), messageBuffer, messageLength,
		0, reinterpret_cast<struct sockaddr*>(&clientAddress), clientAddressLength);

	if (sentLength < 0) {
		ErrorHandler::syserr("sendto");
	}
}

void Broadcaster::broadcastAudio(const char* audioBuffer, size_t dataSize) {
	std::vector<sockaddr_in> activeClients =
		this->lastContactStorage.getActiveClients();

	char messageBuffer[MESSAGE_BUFFER_SIZE];
	for (auto& clientAddress : activeClients) {
		memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
		sendMessage(AUDIO, clientAddress, messageBuffer, audioBuffer, dataSize);
	}
}

void Broadcaster::broadcastMetadata(const char* metadataBuffer,
									size_t dataSize) {
	std::vector<sockaddr_in> activeClients =
		this->lastContactStorage.getActiveClients();

	char messageBuffer[MESSAGE_BUFFER_SIZE];
	for (auto& clientAddress : activeClients) {
		memset(messageBuffer, 0, sizeof(char) * MESSAGE_BUFFER_SIZE);
		sendMessage(METADATA, clientAddress, messageBuffer, metadataBuffer,
					dataSize);
	}
}

void Broadcaster::copyContentToBuffer(char* messageBuffer,
									  const char* messageContent,
									  size_t dataSize) {
	for (size_t byte = 0; byte < dataSize; ++byte) {
		messageBuffer[byte] = messageContent[byte];
	}
}

bool Broadcaster::checkReceivedErrorType(ssize_t receivedLength) {
	if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
		return true;  // continue;
	}

	if (receivedLength == 0) {
		return false;  // return;
	} else {         // receivedLength < 0
		ErrorHandler::syserr("recvfrom");
	}
}
