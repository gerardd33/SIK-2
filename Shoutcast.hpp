#ifndef SHOUTCAST_HPP_
#define SHOUTCAST_HPP_

#include "RadioInfo.hpp"

class Shoutcast {
public:
	Shoutcast(RadioInfo& radioInfo, FILE* socketFile);
	void sendRequest();

private:
	RadioInfo& radioInfo;
	FILE* socketFile;

	const char* INITIAL_HEADERS_FORMAT = "GET %s HTTP/1.1\r\n"
										 "Host: %s\r\n"
										 "User-Agent: testhttp_raw\r\n"
										 "Connection: close\r\n";
};

#endif //SHOUTCAST_HPP_
