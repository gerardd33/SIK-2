#include "RequestSender.hpp"

RequestSender::RequestSender(RadioInfo& radioInfo, FILE* socketFile) : radioInfo(radioInfo), socketFile(socketFile) {
}

void RequestSender::sendRequest() {
	if (fprintf(socketFile, INITIAL_HEADERS_FORMAT,
		radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::requestMetadata() {
	if (fprintf(socketFile, METADATA_REQUEST,
				radioInfo.getRadioResourcePath(), radioInfo.getRadioHost()) < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}

void RequestSender::completeRequest() {
	if (fprintf(socketFile, "\r\n") < 0) {
		ErrorHandler::fatal("Sending HTTP request");
	}
}