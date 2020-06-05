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
	bool requestMetadata;
	const char* radioHost;
	const char* radioResourcePath;
	const char* radioPort;
	unsigned int radioTimeout;

	bool broadcasting;
	const char* broadcastPort;
	const char* broadcastMulticastAddress;
	unsigned int broadcastTimeout;

	static const unsigned int DEFAULT_TIMEOUT = 5;

	void parseArguments(int argc, const char** argv);
	void assignArgument(char argumentFlag, const char* argumentValue);
};

#endif //INPUTDATA_HPP_