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

	int getRadioPort() {
		return radioPort;
	}

	int getTimeout() {
		return timeout;
	}

	bool isRequestMetadata() {
		return requestMetadata;
	}


private:
	const char* radioHost;
	const char* radioResourcePath;
	int radioPort;
	int timeout;
	bool requestMetadata;

	static const int DEFAULT_TIMEOUT = 5;
	static void usage();

	void parseArguments(int argc, const char** argv);
};

#endif //RADIOINFO_HPP_
