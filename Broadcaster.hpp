#ifndef BROADCASTER_HPP_
#define BROADCASTER_HPP_

#include "UdpClient.hpp"
#include <netinet/in.h>
#include <thread>
#include <map>

using LastContactMap = std::map<sockaddr_in, long long>;

class Broadcaster {
public:
	explicit Broadcaster(InputData& inputData);
	~Broadcaster();
	void broadcastAudio(const char* audioBuffer, size_t dataSize);
	void broadcastMetadata(const char* metadataBuffer, size_t dataSize);

	void interrupt() {
		interrupted = true;
	}

	void setRadioName(const char* newName) {
		this->radioName = newName;
	}

private:
	static const uint16_t DISCOVER = 1;
	static const uint16_t IAM = 2;
	static const uint16_t KEEPALIVE = 3;
	static const uint16_t AUDIO = 4;
	static const uint16_t METADATA = 6;
	unsigned int MESSAGE_BUFFER_SIZE = 1024;
	const char* UNKNOWN_RADIO_NAME = "unknown";

	InputData& inputData;
	UdpClient udpConnection;
	bool interrupted;
	const char* radioName;

	LastContactMap lastContactMap;
	sockaddr_in getClientAddress(std::pair<sockaddr_in, long long> mapEntry);
	long long getLastContactTime(std::pair<sockaddr_in, long long> mapEntry);

	std::thread clientHandler;
	void handleClients();

	bool checkReceivedErrorType(ssize_t receivedLength);
};

#endif //BROADCASTER_HPP_
