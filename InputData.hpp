#ifndef INPUTDATA_HPP_
#define INPUTDATA_HPP_

#include "ErrorHandler.hpp"
#include <cstdio>
#include <cstring>
#include <cerrno>

class InputData {
public:
	InputData(int argc, const char** argv);
	~InputData();

	bool isRequestMetadata() {
		return requestMetadata;
	}

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

	bool isBroadcasting() {
		return broadcasting;
	}

	bool isMulticast() {
		return multicast;
	}

	const char* getBroadcastMulticastAddress() {
		return broadcastMulticastAddress;
	}

	unsigned int getBroadcastPort() {
		return broadcastPort;
	}

	unsigned int getBroadcastTimeout() {
		return broadcastTimeout;
	}

private:
	static const unsigned int DEFAULT_TIMEOUT = 5;

	bool requestMetadata;
	const char* radioHost;
	const char* radioResourcePath;
	const char* radioPort;
	unsigned int radioTimeout;

	bool broadcasting;
	bool multicast;
	const char* broadcastMulticastAddress;
	unsigned int broadcastPort;
	unsigned int broadcastTimeout;

	void parseArguments(int argc, const char** argv);
	void assignArgument(char argumentFlag, const char* argumentValue);
	static int parseInteger(const char* string);
};

#endif //INPUTDATA_HPP_
