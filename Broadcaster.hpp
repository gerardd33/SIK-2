#ifndef BROADCASTER_HPP_
#define BROADCASTER_HPP_

#include "Environment.hpp"
#include "UdpClient.hpp"
#include "LastContactStorage.hpp"
#include <netinet/in.h>
#include <sys/time.h>
#include <thread>

class Broadcaster {
public:
	explicit Broadcaster(InputData& inputData);
	~Broadcaster();

	void broadcastAudio(const char* audioBuffer, size_t dataSize);
	void broadcastMetadata(const char* metadataBuffer, size_t dataSize);

	void setRadioName(const char* newName);

private:
	static const uint16_t DISCOVER = 1;
	static const uint16_t IAM = 2;
	static const uint16_t KEEPALIVE = 3;
	static const uint16_t AUDIO = 4;
	static const uint16_t METADATA = 6;

	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;
	const size_t HEADER_FIELD_SIZE = 2;
	const size_t MESSAGE_BUFFER_SIZE = DEFAULT_DATA_CHUNK_SIZE + 4 * HEADER_FIELD_SIZE;

	const char* UNKNOWN_RADIO_NAME = "unknown";

	InputData& inputData;
	UdpClient udpConnection;
	const char* radioName;

	LastContactStorage lastContactStorage;

	std::thread clientHandler;
	void handleClients();

	std::mutex waitForRadioMutex;

	static void copyContentToBuffer(char* messageBuffer, const char* messageContent, size_t dataSize);
	void sendMessage(uint16_t messageType, sockaddr_in clientAddress, char* messageBuffer,
		const char* messageContent, size_t contentSize);
	bool checkReceivedErrorType(ssize_t receivedLength);
};

#endif //BROADCASTER_HPP_
