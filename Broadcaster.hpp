#ifndef BROADCASTER_HPP_
#define BROADCASTER_HPP_

#include <netinet/in.h>
#include <sys/time.h>
#include <thread>

#include "Environment.hpp"
#include "LastContactStorage.hpp"
#include "UdpClient.hpp"

class Broadcaster {
public:
	explicit Broadcaster(InputData& inputData);
	~Broadcaster();

	void setRadioName(const char* newName);

	void broadcastAudio(const char* audioBuffer, size_t dataSize);
	void broadcastMetadata(const char* metadataBuffer, size_t dataSize);

private:
	static const uint16_t DISCOVER = 1;
	static const uint16_t IAM = 2;
	static const uint16_t KEEPALIVE = 3;
	static const uint16_t AUDIO = 4;
	static const uint16_t METADATA = 6;

	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;
	const size_t HEADER_FIELD_SIZE = 2;
	const size_t MESSAGE_BUFFER_SIZE =
		DEFAULT_DATA_CHUNK_SIZE + 4 * HEADER_FIELD_SIZE;
	const char* UNKNOWN_RADIO_NAME = "unknown";

	InputData& inputData;

	UdpClient udpConnection;
	const char* radioName;
	LastContactStorage lastContactStorage;

	std::thread clientHandler;
	std::mutex waitForRadioMutex;

	void handleClients();
	void sendMessage(uint16_t messageType, sockaddr_in& clientAddress,
					 char* messageBuffer, const char* messageContent,
					 size_t contentSize);

	static void copyContentToBuffer(char* messageBuffer,
									const char* messageContent, size_t dataSize);
	static bool checkReceivedErrorType(ssize_t receivedLength);
};

#endif  // BROADCASTER_HPP_
