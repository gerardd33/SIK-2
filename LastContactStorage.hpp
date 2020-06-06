#ifndef LASTCONTACTSTORAGE_HPP_
#define LASTCONTACTSTORAGE_HPP_

#include "InputData.hpp"
#include <netinet/in.h>
#include <sys/time.h>
#include <mutex>
#include <vector>
#include <thread>
#include <map>

using lastContactMapEntry = std::pair<const sockaddr_in, long long>;

class LastContactStorage {
public:
	explicit LastContactStorage(InputData& inputData) : inputData(inputData) {}

	std::vector<sockaddr_in> getActiveClients();
	void updateLastContact(sockaddr_in clientAddress);

private:
	InputData& inputData;
	std::mutex storageMutex;

	struct compareClientAddress {
		bool operator()(const sockaddr_in& address1, const sockaddr_in& address2) {
			return std::make_pair(getAddress(address1), getPort(address1)) <
				std::make_pair(getAddress(address2), getPort(address2));
		}
	};

	std::map<sockaddr_in, long long, compareClientAddress> lastContactMap;

	static in_addr_t getAddress(const sockaddr_in& addressStruct) {
		return addressStruct.sin_addr.s_addr;
	}

	static in_port_t getPort(const sockaddr_in& addressStruct) {
		return addressStruct.sin_port;
	}

	static sockaddr_in getClientAddress(lastContactMapEntry& mapEntry) {
		return mapEntry.first;
	}

	static long long getLastContactTime(lastContactMapEntry& mapEntry) {
		return mapEntry.second;
	}

	static long long getCurrentMilliseconds();

};

#endif //LASTCONTACTSTORAGE_HPP_
