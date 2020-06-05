#include "ResponseProcessor.hpp"

void ResponseProcessor::readStatusLine() {
	char* protocoleVersion = nullptr;
	char* statusMessage = nullptr;
	int statusCode;

	fscanf(radioSocketFile, "%m[^ ] %d %m[^\r\n]\n",
		&protocoleVersion, &statusCode, &statusMessage);

	free(protocoleVersion);
	if (statusCode != 200 || strcmp(statusMessage, "OK") != 0) {
		fprintf(stderr, "%d %s\n", statusCode, statusMessage);
		free(statusMessage);
		ErrorHandler::fatal("Requesting from server");
	}

	free(statusMessage);
}

void ResponseProcessor::readHeaders() {
	char* line = nullptr;
	char* parsedHeader = nullptr;
	size_t bufferSize = 0;

	while (true) {
		if (getline(&line, &bufferSize, radioSocketFile) == -1) {
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
	if (ferror(radioSocketFile)) {
		ErrorHandler::syserr("fread");
	}

	return feof(radioSocketFile);
}

void ResponseProcessor::printString(FILE* stream, char* string, size_t size) {
	for (size_t byte = 0; byte < size; ++byte) {
		fprintf(stream, "%c", string[byte]);
	}
}

void ResponseProcessor::readAudioBlock(char* audioBuffer) {
	size_t bytesRead = fread(audioBuffer, 1, this->dataChunkSize, radioSocketFile);
	if (bytesRead < this->dataChunkSize) {
		ErrorHandler::fatal("Timeout");
	}

	processAudio(audioBuffer, this->dataChunkSize);
}

void ResponseProcessor::readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer) {
	if (!this->inputData.isRequestMetadata()) {
		return;
	}

	metadataSizeBuffer[0] = 0;
	size_t bytesRead = fread(metadataSizeBuffer, 1, 1, radioSocketFile);
	if (bytesRead < 1) {
		ErrorHandler::fatal("Processing server response");
	}

	auto metadataSize = static_cast<size_t>(metadataSizeBuffer[0]);
	if (metadataSize == 0) {
		return;
	}

	metadataSize *= METADATA_BLOCKSIZE_FACTOR;
	bytesRead = fread(metadataBuffer, 1, metadataSize, radioSocketFile);
	if (bytesRead < metadataSize) {
		ErrorHandler::fatal("Processing server response");
	}

	processMetadata(metadataBuffer, metadataSize);
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
	int sscanfResult = sscanf(line, "icy-metaint%*[: ]%zu[\r\n]", &readValue);

	if (sscanfResult > 0) {
		this->dataChunkSize = readValue;
	}
}

ResponseProcessor::ResponseProcessor(InputData& inputData, FILE* radioSocketFile, FILE* broadcastSocketFile) :
	inputData(inputData), broadcastSocketFile(broadcastSocketFile) {
	if (this->inputData.isRequestMetadata()) {
		this->dataChunkSize = -1;
	} else {
		this->dataChunkSize = DEFAULT_DATA_CHUNK_SIZE;
	}
}

void ResponseProcessor::processAudio(char* audioBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		// TODO Redirect to broadcaster
	} else {
		printString(stdout, audioBuffer, dataSize);
	}
}

void ResponseProcessor::processMetadata(char* metadataBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		// TODO Redirect to broadcaster
	} else {
		printString(stderr, metadataBuffer, dataSize);
		fprintf(stderr, "\n");
	}
}

void ResponseProcessor::processServerResponse() {
	readStatusLine();
	readHeaders();
	readData();
}
