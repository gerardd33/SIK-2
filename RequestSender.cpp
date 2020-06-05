#include "RequestSender.hpp"

void RequestSender::sendInitialHeaders() {
	if (fprintf(radioSocketFile, INITIAL_HEADERS_FORMAT,
		radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::requestMetadata() {
	if (fprintf(radioSocketFile, METADATA_REQUEST, static_cast<int>(this->radioInfo.isRequestMetadata()),
				radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::completeRequest() {
	if (fprintf(radioSocketFile, "\r\n") < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::sendRequest() {
	sendInitialHeaders();
	requestMetadata();
	completeRequest();
}
