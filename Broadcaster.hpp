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

private:
	InputData& inputData;
	FILE* broadcastSocketFile;
	bool interrupted;

	LastContactMap lastContactMap;
	sockaddr_in getClientAddress(std::pair<sockaddr_in, long long> mapEntry);
	long long getLastContactTime(std::pair<sockaddr_in, long long> mapEntry);

	std::thread clientHandler;
	void handleClients();
};

#endif //BROADCASTER_HPP_
