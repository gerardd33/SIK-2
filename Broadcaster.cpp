#include "Broadcaster.hpp"

sockaddr_in Broadcaster::getClientAddress(std::pair<sockaddr_in, long long> mapEntry) {
	return mapEntry.first;
}

long long Broadcaster::getLastContactTime(std::pair<sockaddr_in, long long> mapEntry) {
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
}

void Broadcaster::handleClients() {
	// TODO podziel lepiej na funkcje, przenies niektore odpowiedzialnosci do UDP

	struct sockaddr_in clientAddress;
	char messageBuffer[MESSAGE_BUFFER_SIZE];

	int socketDescriptor = this->udpConnection.getSocketDescriptor();
	while (!this->interrupted) {
		auto clientAddressLength = static_cast<socklen_t>(sizeof(clientAddress));
		ssize_t receivedLength = recvfrom(socketDescriptor, messageBuffer, sizeof(messageBuffer),
							  0, (struct sockaddr*) &clientAddress, &clientAddressLength);

		if (receivedLength <= 0) {
			if (checkReceivedErrorType(receivedLength)) {
				return;
			} else {
				continue;
			}
		}

		// zmien nazwe bufora

		// TODO zmien i ladniej
		uint16_t messageType = ntohs(*((uint16_t*) messageBuffer));
		switch (messageType) {
			case DISCOVER:
				*((uint16_t*) messageBuffer) = htons(IAM);
				*((uint16_t*) (messageBuffer + 2)) = htons(radioname.size);
				strcpy(messageBuffer + 4, radioname);

				responseLength = radioname.size + 4
				ssize_t sentLength = sendto(socketDescriptor, messageFromClientBuffer, responseLength, 0,
											(struct sockaddr*) &clientAddress, clientAddressLength);

				if (sentLength < 0) {
					//syserr
				}

				timeval tp;
				timeMapMutex.lock();
				gettimeofday(&tp, nullptr);
				uint64_t ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
				timeMap[client_address] = ms;
				timeMapMutex.unlock();

			case KEEPALIVE:
				// ...
			default:
				// ...
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

void Broadcaster::broadcastAudio(const char* audioBuffer, size_t dataSize) {
	fprintf(stderr, "broadcasting audio\n");
}

void Broadcaster::broadcastMetadata(const char* metadataBuffer, size_t dataSize) {
	fprintf(stderr, "broadcasting metadata\n");
}
