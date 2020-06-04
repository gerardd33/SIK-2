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

	if (this->dataChunkSize == -1) {
		ErrorHandler::fatal("Processing server response");
	}
}

bool ResponseProcessor::checkIfFinished() {
	if (ferror(socketFile)) {
		ErrorHandler::syserr("fread");
	}

	return feof(socketFile);
}

void ResponseProcessor::printString(FILE* stream, char* string, size_t size) {
	for (size_t byte = 0; byte < size; ++byte) {
		fprintf(stream, "%c", string[byte]);
	}
}

void ResponseProcessor::readAudioBlock(char* audioBuffer) {
	size_t bytesRead = fread(audioBuffer, 1, this->dataChunkSize, socketFile);
	if (bytesRead < this->dataChunkSize) {
		ErrorHandler::fatal("Timeout");
	}

	printString(stdout, audioBuffer, this->dataChunkSize);
}

void ResponseProcessor::readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer) {
	if (!this->requestMetadata) {
		return;
	}

	metadataSizeBuffer[0] = 0;
	size_t bytesRead = fread(metadataSizeBuffer, 1, 1, socketFile);
	if (bytesRead < 1) {
		ErrorHandler::fatal("Processing server response");
	}

	size_t metadataSize = static_cast<size_t>(metadataSizeBuffer[0]);

	if (metadataSize == 0) {
		return;
	}

	metadataSize *= METADATA_BLOCKSIZE_FACTOR;
	bytesRead = fread(metadataBuffer, 1, metadataSize, socketFile);
	if (bytesRead < metadataSize) {
		ErrorHandler::fatal("Processing server response");
	}

	printString(stderr, metadataBuffer, metadataSize);
	fprintf(stderr, "\n");
}

void ResponseProcessor::readData() {
	char* audioBuffer = (char*) malloc(sizeof(char) * this->dataChunkSize);
	char metadataSizeBuffer[1];
	char metadataBuffer[METADATA_MAX_LENGTH];

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
}

void ResponseProcessor::convertHeaderNameToLowercase(char* line) {
	for (size_t index = 0; line[index] != 0 && line[index] != ':'; ++index) {
		line[index] = static_cast<char>(tolower(line[index]));
	}
}

void ResponseProcessor::checkIfMetadataInterval(char* line) {
	size_t readValue;
	int sscanfResult = sscanf(line, "icy-metaint:%zu[\r\n]", &readValue);
	if (sscanfResult > 0) {
		this->dataChunkSize = readValue;
	}
}

ResponseProcessor::ResponseProcessor(RadioInfo& radioInfo, FILE* socketFile) : socketFile(socketFile),
	requestMetadata(radioInfo.isRequestMetadata()) {
	if (this->requestMetadata) {
		this->dataChunkSize = -1;
	} else {
		this->dataChunkSize = DEFAULT_DATA_CHUNK_SIZE;
	}
}

void ResponseProcessor::processServerResponse() {
	if (!readStatusLine()) {
		return;
	}
	readHeaders();
	readData();
}
