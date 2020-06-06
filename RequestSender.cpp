#include "RequestSender.hpp"

void RequestSender::sendRequest() {
	sendInitialHeaders();
	requestMetadata();
	completeRequest();
}

void RequestSender::sendInitialHeaders() {
	if (fprintf(this->tcpClient.getSocketFile(), INITIAL_HEADERS_FORMAT,
		inputData.getRadioResourcePath(), inputData.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::requestMetadata() {
	if (fprintf(this->tcpClient.getSocketFile(), METADATA_REQUEST, static_cast<int>(this->inputData.isRequestMetadata()),
				inputData.getRadioResourcePath(), inputData.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::completeRequest() {
	if (fprintf(this->tcpClient.getSocketFile(), "%s", CRLF) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}
