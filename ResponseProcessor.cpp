#include "ResponseProcessor.hpp"

bool ResponseProcessor::readStatusLine() {
	char* protocoleVersion = nullptr;
	char* statusMessage = nullptr;
	int statusCode;

	fscanf(socketFile, "%m[^ ] %d %m[^\r\n]\n",
		&protocoleVersion, &statusCode, &statusMessage);

	// TODO usun debug
	fprintf(stderr, "status line: %d %s\n", statusCode, statusMessage);

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

		// TODO usun debug
		fprintf(stderr, "header: %s\n", line);

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

bool ResponseProcessor::checkIfFinished() {
	if (ferror(socketFile)) {
		ErrorHandler::syserr("fread");
	}

	return feof(socketFile);
}

void ResponseProcessor::readAudioBlock(char* audioBuffer) {
	size_t bytesRead = fread(audioBuffer, this->metadataInterval, sizeof(audioBuffer), socketFile);
	if (bytesRead < this->metadataInterval) {
		fprintf(stderr, "bytes read: %d out of %d\n", bytesRead, this->metadataInterval);
		ErrorHandler::fatal("Timeout");
	}

	// TODO usun debug
	fprintf(stderr, "DATA start: \n");
	printf("%s\n", audioBuffer);
	fprintf(stderr, "DATA end: \n");
}

void ResponseProcessor::readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer) {
	size_t bytesRead = fread(metadataSizeBuffer, 1, sizeof(metadataSizeBuffer), socketFile);
	if (bytesRead < 1) {
		ErrorHandler::fatal("Processing server response");
	}

	auto metadataSize = static_cast<size_t>(strtol(metadataSizeBuffer, nullptr, 10));
	if (metadataSize == 0) {
		if (errno != 0) {
			ErrorHandler::fatal("Processing server response");
		} else {
			return;
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

void ResponseProcessor::readData() {
	char* audioBuffer = (char*) malloc(sizeof(char) * (this->metadataInterval + 1));
	char* metadataSizeBuffer = (char*) malloc(sizeof(char) * 2); // 1 byte + null-termination.
	char* metadataBuffer = (char* )malloc(sizeof(char) * (METADATA_MAX_LENGTH + 1));
	// Null-termination for writing.
	audioBuffer[this->metadataInterval] = 0;
	metadataSizeBuffer[1] = 0;

	while (true) {
		readAudioBlock(audioBuffer);
		if (checkIfFinished()) {
			break;
		}

		readMetadataBlock(metadataSizeBuffer, metadataBuffer);
		if (checkIfFinished()) {
			break;
		}
	}

	free(audioBuffer);
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
	int sscanfResult = sscanf(line, "icy-metaint:%d[\r\n]", &readValue);
	if (sscanfResult > 0) {
		printf("value: %d\n", readValue);
		this->metadataInterval = readValue;
	}
}

void ResponseProcessor::processServerResponse() {
	if (!readStatusLine()) {
		return;
	}
	// TODO usun debug
	fprintf(stderr, "Status line read\n");
	readHeaders();
	fprintf(stderr, "Headers read\n");
	readData();
	fprintf(stderr, "Data read\n");
}

ResponseProcessor::ResponseProcessor(RadioInfo& radioInfo, FILE* socketFile) : socketFile(socketFile),
	requestMetadata(radioInfo.isRequestMetadata()) {
	if (this->requestMetadata) {
		this->metadataInterval = -1;
	} else {
		this->metadataInterval = DEFAULT_CHUNK_SIZE;
	}
}
