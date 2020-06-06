#ifndef REQUESTSENDER_HPP_
#define REQUESTSENDER_HPP_

#include "InputData.hpp"
#include "TcpClient.hpp"

class RequestSender {
public:
	RequestSender(InputData& inputData, TcpClient& tcpClient) : inputData(inputData), tcpClient(tcpClient) {}
	void sendRequest();

private:
	const char* INITIAL_HEADERS_FORMAT = "GET %s HTTP/1.0\r\n"
										 "Host: %s\r\n"
										 "User-Agent: Player\r\n"
										 "Connection: close\r\n";

	const char* METADATA_REQUEST = "Icy-MetaData:%d\r\n";
	const char* CRLF = "\r\n";

	InputData& inputData;
	TcpClient& tcpClient;

	void sendInitialHeaders();
	void requestMetadata();
	void completeRequest();
};

#endif //REQUESTSENDER_HPP_
