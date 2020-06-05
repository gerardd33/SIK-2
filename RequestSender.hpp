#ifndef REQUESTSENDER_HPP_
#define REQUESTSENDER_HPP_

#include "InputData.hpp"

class RequestSender {
public:
	RequestSender(InputData& radioInfo, FILE* radioSocketFile) : radioInfo(radioInfo), radioSocketFile(radioSocketFile) {}
	void sendRequest();

private:
	InputData& radioInfo;
	FILE* radioSocketFile;

	const char* INITIAL_HEADERS_FORMAT = "GET %s HTTP/1.0\r\n"
										 "Host: %s\r\n"
		   								 "User-Agent: Player\r\n"
										 "Connection: close\r\n";

	const char* METADATA_REQUEST = "Icy-MetaData:%d\r\n";

	void sendInitialHeaders();
	void requestMetadata();
	void completeRequest();
};

#endif //REQUESTSENDER_HPP_
