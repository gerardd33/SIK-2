#ifndef RADIOINFO_HPP_
#define RADIOINFO_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>


class RadioInfo {
public:
	RadioInfo(int argc, const char** argv);

	const char* getRadioHost() {
		return radioHost;
	}

	const char* getRadioResourcePath() {
		return radioResourcePath;
	}

	unsigned int getRadioPort() {
		return radioPort;
	}

	unsigned int getTimeout() {
		return timeout;
	}

	bool isRequestMetadata() {
		return requestMetadata;
	}


private:
	const char* radioHost;
	const char* radioResourcePath;
	unsigned int radioPort;
	unsigned int timeout;
	bool requestMetadata;

	static const unsigned int DEFAULT_TIMEOUT = 5;
	static void usage();
};

#endif //RADIOINFO_HPP_
