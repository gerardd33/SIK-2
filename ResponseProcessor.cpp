#include "ResponseProcessor.hpp"

bool ResponseProcessor::readStatusLine() {
	char* protocoleVersion = nullptr;
	char* statusMessage = nullptr;
	int statusCode;

	fscanf(socketFile, "%m[^ ] %d %m[^\r\n]\n",
		&protocoleVersion, &statusCode, &statusMessage);

	free(protocoleVersion);
	if (statusCode != 200 || strcmp(statusMessage, "OK") != 0) {
		fprintf(stderr, "%d %s\n", statusCode, statusMessage);
		free(statusMessage);
		return false;
	}

	free(statusMessage);
	return true;
}

void ResponseProcessor::readHeaders() {
	char* line = nullptr;
	char* parsedHeader = nullptr;
	size_t bufferSize = 0;

	while (true) {
		if (getline(&line, &bufferSize, socketFile) == -1) {
			ErrorHandler::syserr("getline");
		}

		if (strcmp(line, "\r\n") == 0) {
			break;
		}

		convertHeaderNameToLowercase(line);
		checkIfMetadataInterval(line);
	}

	free(line);
	free(parsedHeader);

	if (this->metadataInterval == -1) {
		ErrorHandler::fatal("Processing server response");
	}
}

void ResponseProcessor::readData() {
	// TODO demultipleksowanie metadanych
	std::vector<char> buffer(this->metadataInterval, 0);

	size_t resourceLength = 0;

	// TODO dokoncz, w szczegolnosci uwazaj na metadane i rozmiary blokow
}

void ResponseProcessor::convertHeaderNameToLowercase(char* line) {
	for (size_t index = 0; line[index] != 0 && line[index] != ':'; ++index) {
		line[index] = static_cast<char>(tolower(line[index]));
	}
}

void ResponseProcessor::checkIfMetadataInterval(char* line) {
	int readValue;
	int sscanfResult = sscanf(line, "icy-metaint:[ ]%d", &readValue);
	if (sscanfResult > 0) {
		this->metadataInterval = readValue;
	}
}

void ResponseProcessor::processServerResponse() {
	if (!readStatusLine()) {
		return;
	}
	readHeaders();
	readData();
}
