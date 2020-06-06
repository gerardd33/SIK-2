#include "ResponseProcessor.hpp"

bool ResponseProcessor::readStatusLine() {
	char* protocoleVersion = nullptr;
	char* statusMessage = nullptr;
	int statusCode;

	fscanf(radioSocketFile, "%m[^ ] %d %m[^\r\n]\n",
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
		if (getline(&line, &bufferSize, radioSocketFile) == -1) {
			ErrorHandler::syserr("getline");
		}

		if (strcmp(line, "\r\n") == 0) {
			break;
		}

		convertHeaderNameToLowercase(line);
		checkIfMetadataInterval(line);
		checkIfRadioName(line);
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
	int toBeRead = this->dataChunkSize;

	while (toBeRead > 0) {
		size_t bytesRead = fread(audioBuffer, 1, std::min(toBeRead,
			static_cast<int>(DEFAULT_DATA_CHUNK_SIZE)), radioSocketFile);
		if (bytesRead < std::min(toBeRead, static_cast<int>(DEFAULT_DATA_CHUNK_SIZE))) {
			ErrorHandler::fatal("Timeout");
		}

		processAudio(audioBuffer, std::min(toBeRead, static_cast<int>(DEFAULT_DATA_CHUNK_SIZE)));
		toBeRead -= DEFAULT_DATA_CHUNK_SIZE;
	}
}

void ResponseProcessor::readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer) {
	ErrorHandler::debug("kurwa");

	if (!this->inputData.isRequestMetadata()) {
		ErrorHandler::debug("tam");
		return;
	}

	ErrorHandler::debug("tutaj");

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

	ErrorHandler::debug("1");

	while (true) {
		ErrorHandler::debug("2");
		readAudioBlock(audioBuffer);
		ErrorHandler::debug("3");
		if (checkIfFinished()) {
			break;
		}
		ErrorHandler::debug("4");

		readMetadataBlock(metadataSizeBuffer, metadataBuffer);
		if (checkIfFinished()) {
			break;
		}
		ErrorHandler::debug("5");
	}

	// TODO zadbaj zeby to sie wykonalo albo daj jakos do destruktora
	free(audioBuffer);
}

void ResponseProcessor::convertHeaderNameToLowercase(char* line) {
	for (size_t index = 0; line[index] != 0 && line[index] != ':'; ++index) {
		line[index] = static_cast<char>(tolower(line[index]));
	}
}

void ResponseProcessor::checkIfMetadataInterval(char* line) {
	size_t readValue;
	// TODO sprawdz czy zmienic [\r\n] na %*
	int sscanfResult = sscanf(line, "icy-metaint%*[: ]%zu[\r\n]", &readValue);

	if (sscanfResult > 0) {
		this->dataChunkSize = readValue;
	}
}

void ResponseProcessor::checkIfRadioName(char* line) {
	char* readValue = nullptr;
	// TODO sprawdz czy zmienic [\r\n] na %*
	int sscanfResult = sscanf(line, "icy-name%*[: ]%ms[\r\n]", &readValue);

	if (sscanfResult > 0) {
		this->broadcaster.setRadioName(readValue);
	}

	// TODO czy nie wywala jesli nullptr?
	free(readValue);
}

ResponseProcessor::ResponseProcessor(InputData& inputData, FILE* radioSocketFile, Broadcaster& broadcaster) :
	inputData(inputData), radioSocketFile(radioSocketFile), broadcaster(broadcaster) {
	if (this->inputData.isRequestMetadata()) {
		this->dataChunkSize = -1;
	} else {
		this->dataChunkSize = DEFAULT_DATA_CHUNK_SIZE;
	}
}

void ResponseProcessor::processAudio(char* audioBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		this->broadcaster.broadcastAudio(audioBuffer, dataSize);
	} else {
		printString(stdout, audioBuffer, dataSize);
	}
}

void ResponseProcessor::processMetadata(char* metadataBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		this->broadcaster.broadcastMetadata(metadataBuffer, dataSize);
	} else {
		printString(stderr, metadataBuffer, dataSize);
		fprintf(stderr, "\n");
	}
}

void ResponseProcessor::processServerResponse() {
	if (!readStatusLine()) {
		return;
	}
	readHeaders();
	readData();
}
