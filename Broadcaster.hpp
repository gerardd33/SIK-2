#ifndef BROADCASTER_HPP_
#define BROADCASTER_HPP_

#include "UdpClient.hpp"
#include <netinet/in.h>
#include <sys/time.h>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

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
	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;
	unsigned int HEADER_FIELD_SIZE = 2;
	const char* UNKNOWN_RADIO_NAME = "unknown";

	InputData& inputData;
	UdpClient udpConnection;
	bool interrupted;
	const char* radioName;

	// TODO daj mape do osobnej klasy, moze tez tam tego demona utrzymujacego

	static in_addr_t getAddress(const sockaddr_in& addressStruct) {
		return addressStruct.sin_addr.s_addr;
	}

	static in_port_t getPort(const sockaddr_in& addressStruct) {
		return addressStruct.sin_port;
	}

	struct compareClientAddress {
		bool operator()(const sockaddr_in& address1, const sockaddr_in& address2) {
			return std::make_pair(getAddress(address1), getPort(address1)) <
				std::make_pair(getAddress(address2), getPort(address2));
		}
	};

	std::map<sockaddr_in, long long, compareClientAddress> lastContactMap;
	sockaddr_in getClientAddress(std::pair<sockaddr_in, long long> mapEntry);
	long long getLastContactTime(std::pair<sockaddr_in, long long> mapEntry);
	void updateLastContact(sockaddr_in clientAddress);

	std::thread clientHandler;
	void handleClients();
	std::mutex lastContactMapMutex;

	void sendGreeting(char* messageBuffer, struct sockaddr_in clientAddress, socklen_t clientAddressLength);
	bool checkReceivedErrorType(ssize_t receivedLength);
	static long long getCurrentMiliseconds();
};

#endif //BROADCASTER_HPP_
