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
	char* dataBuffer = (char*) malloc(sizeof(char) * (this->metadataInterval + 1));
	char* metadataSizeBuffer = (char*) malloc(sizeof(char) * 2); // 1 byte + null-termination.
	char* metadataBuffer = (char* )malloc(sizeof(char) * (METADATA_MAX_LENGTH + 1));
	// Null-termination for writing.
	dataBuffer[this->metadataInterval] = 0;
	metadataSizeBuffer[1] = 0;

	while (true) {
		size_t bytesRead = fread(dataBuffer, this->metadataInterval, sizeof(dataBuffer), socketFile);
		if (bytesRead < this->metadataInterval) {
			ErrorHandler::fatal("Timeout");
		}

		if (ferror(socketFile)) {
			ErrorHandler::syserr("fread");
		}

		if (feof(socketFile)) {
			break;
		}

		// TODO usun debug
		fprintf(stderr, "DATA start: \n");
		printf("%s\n", dataBuffer);
		fprintf(stderr, "DATA end: \n");

		bytesRead = fread(metadataSizeBuffer, 1, sizeof(metadataSizeBuffer), socketFile);
		if (bytesRead < 1) {
			ErrorHandler::fatal("Processing server response");
		}

		auto metadataSize = static_cast<size_t>(strtol(metadataSizeBuffer, nullptr, 10));
		if (metadataSize == 0) {
			if (errno != 0) {
				ErrorHandler::fatal("Processing server response");
			} else {
				continue;
			}
		}

		metadataSize *= METADATA_BLOCKSIZE_FACTOR;
		bytesRead = fread(metadataBuffer, metadataSize, sizeof(metadataBuffer), socketFile);
		if (bytesRead < metadataSize) {
			ErrorHandler::fatal("Processing server response");
		}

		metadataBuffer[metadataSize] = 0; // Null-termination for writing.

		// TODO usun debug
		fprintf(stderr, "METADATA start: \n");
		fprintf(stderr, "%s\n", metadataBuffer);
		fprintf(stderr, "METADATA end: \n");
	}

	free(dataBuffer);
	free(metadataSizeBuffer);
	free(metadataBuffer);
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
