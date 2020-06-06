#include "ResponseProcessor.hpp"

ResponseProcessor::ResponseProcessor(InputData& inputData, TcpClient& tcpClient, Broadcaster* broadcaster) :
	inputData(inputData), tcpClient(tcpClient), broadcaster(broadcaster) {
	if (this->inputData.isRequestMetadata()) {
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

bool ResponseProcessor::readStatusLine() {
	char* statusMessage = nullptr;
	int statusCode;

	if (fscanf(this->tcpClient.getSocketFile(), "%*[^ ] %d %m[^\r\n]\n", &statusCode, &statusMessage) < 0) {
		ErrorHandler::fatal("Processing server response");
	}

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
	size_t bufferSize = 0;

	while (!Environment::interrupted) {
		if (getline(&line, &bufferSize, this->tcpClient.getSocketFile()) == -1) {
			free(line);
			ErrorHandler::syserr("getline");
		}

		if (strcmp(line, CRLF) == 0) {
			free(line);
			break;
		}

		convertHeaderNameToLowercase(line);
		checkIfMetadataInterval(line);

		if (this->inputData.isBroadcasting()) {
			checkIfRadioName(line);
		}
	}

	free(line);
	// Did not find metadata interval information in headers.
	if (this->dataChunkSize == -1) {
		ErrorHandler::fatal("Processing server response");
	}
}

void ResponseProcessor::checkIfMetadataInterval(char* line) {
	size_t readValue;
	int readItems = sscanf(line, "icy-metaint%*[: ]%zu%*[\r\n]", &readValue);

	if (readItems > 0) {
		this->dataChunkSize = readValue;
	}
}

void ResponseProcessor::checkIfRadioName(char* line) {
	char* readValue = nullptr;
	int readItems = sscanf(line, "icy-name%*[: ]%ms%*[\r\n]", &readValue);

	if (readItems > 0) {
		this->broadcaster->setRadioName(readValue);
	}

	free(readValue);
}

void ResponseProcessor::readData() {
	char* audioBuffer = (char*) malloc(sizeof(char) * this->dataChunkSize);
	char metadataSizeBuffer[1];
	char metadataBuffer[METADATA_MAX_LENGTH];

	while (!Environment::interrupted) {
		readAudioBlock(audioBuffer);
		if (checkIfFinished()) {
			free(audioBuffer);
			break;
		}

		readMetadataBlock(metadataSizeBuffer, metadataBuffer);
		if (checkIfFinished()) {
			free(audioBuffer);
			break;
		}
	}

	free(audioBuffer);
}

bool ResponseProcessor::checkIfFinished() {
	if (ferror(this->tcpClient.getSocketFile())) {
		ErrorHandler::syserr("fread");
	}

	return feof(this->tcpClient.getSocketFile());
}

void ResponseProcessor::readAudioBlock(char* audioBuffer) {
	int toBeRead = this->dataChunkSize;

	while (toBeRead > 0) {
		size_t bytesRead = fread(audioBuffer, 1, std::min(toBeRead,
			static_cast<int>(DEFAULT_DATA_CHUNK_SIZE)), this->tcpClient.getSocketFile());
		if (bytesRead < std::min(toBeRead, static_cast<int>(DEFAULT_DATA_CHUNK_SIZE))) {
			ErrorHandler::fatal("Timeout");
		}

		processAudio(audioBuffer, std::min(toBeRead, static_cast<int>(DEFAULT_DATA_CHUNK_SIZE)));
		toBeRead -= DEFAULT_DATA_CHUNK_SIZE;
	}
}

void ResponseProcessor::readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer) {
	if (!this->inputData.isRequestMetadata()) {
		return;
	}

	metadataSizeBuffer[0] = 0;
	size_t bytesRead = fread(metadataSizeBuffer, 1, 1, this->tcpClient.getSocketFile());
	if (bytesRead < 1) {
		ErrorHandler::fatal("Processing server response");
	}

	auto metadataSize = static_cast<size_t>(metadataSizeBuffer[0]);
	if (metadataSize == 0) {
		return;
	}

	metadataSize *= METADATA_BLOCKSIZE_FACTOR;
	bytesRead = fread(metadataBuffer, 1, metadataSize, this->tcpClient.getSocketFile());
	if (bytesRead < metadataSize) {
		ErrorHandler::fatal("Processing server response");
	}

	processMetadata(metadataBuffer, metadataSize);
}

void ResponseProcessor::processAudio(char* audioBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		this->broadcaster->broadcastAudio(audioBuffer, dataSize);
	} else {
		printString(stdout, audioBuffer, dataSize);
	}
}

void ResponseProcessor::processMetadata(char* metadataBuffer, size_t dataSize) {
	if (this->inputData.isBroadcasting()) {
		this->broadcaster->broadcastMetadata(metadataBuffer, dataSize);
	} else {
		printString(stderr, metadataBuffer, dataSize);
		fprintf(stderr, "\n");
	}
}

void ResponseProcessor::convertHeaderNameToLowercase(char* line) {
	for (size_t index = 0; line[index] != 0 && line[index] != ':'; ++index) {
		line[index] = static_cast<char>(tolower(line[index]));
	}
}

void ResponseProcessor::printString(FILE* stream, char* string, size_t size) {
	for (size_t byte = 0; byte < size; ++byte) {
		fprintf(stream, "%c", string[byte]);
	}
}
