#include "RequestSender.hpp"

void RequestSender::sendInitialHeaders() {
	if (fprintf(socketFile, INITIAL_HEADERS_FORMAT,
		radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::requestMetadata() {
	if (fprintf(socketFile, METADATA_REQUEST, (int) this->radioInfo.isRequestMetadata(),
				radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::completeRequest() {
	if (fprintf(socketFile, "\r\n") < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::sendRequest() {
	sendInitialHeaders();
	requestMetadata();
	completeRequest();
}
