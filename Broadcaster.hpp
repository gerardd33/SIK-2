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

private:
	InputData& inputData;
	FILE* broadcastSocketFile;

	LastContactMap lastContactMap;
	sockaddr_in getClientAddress(std::pair<sockaddr_in, long long> mapEntry);
	long long getLastContactTime(std::pair<sockaddr_in, long long> mapEntry);

	void handleClients();
};

#endif //BROADCASTER_HPP_
