#ifndef RADIOINFO_HPP_
#define RADIOINFO_HPP_

#include "ErrorHandler.hpp"
#include <cstdio>
#include <cstring>
#include <cerrno>

class RadioInfo {
public:
	RadioInfo(int argc, const char** argv);
	~RadioInfo();

	const char* getRadioHost() {
		return radioHost;
	}

	const char* getRadioResourcePath() {
		return radioResourcePath;
	}

	const char* getRadioPort() {
		return radioPort;
	}

	unsigned int getRadioTimeout() {
		return radioTimeout;
	}

	bool isRequestMetadata() {
		return requestMetadata;
	}

	const char* getBroadcastPort() {
		return broadcastPort;
	}

	const char* getBroadcastMulticastAddress() {
		return broadcastMulticastAddress;
	}

	unsigned int getBroadcastTimeout() {
		return broadcastTimeout;
	}

private:
	const char* radioHost;
	const char* radioResourcePath;
	const char* radioPort;
	unsigned int radioTimeout;
	bool requestMetadata;
	const char* broadcastPort;
	const char* broadcastMulticastAddress;
	unsigned int broadcastTimeout;

	static const unsigned int DEFAULT_RADIO_TIMEOUT = 5;

	void parseArguments(int argc, const char** argv);
	void assignArgument(char argumentFlag, const char* argumentValue);
};

#endif //RADIOINFO_HPP_
