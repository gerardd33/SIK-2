#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "Environment.hpp"
#include "ErrorHandler.hpp"
#include "InputData.hpp"
#include "Broadcaster.hpp"
#include "TcpClient.hpp"
#include <cctype>
#include <vector>

class ResponseProcessor {
public:
	ResponseProcessor(InputData& inputData, TcpClient& tcpClient, Broadcaster* broadcaster);
	void processServerResponse();

private:
	const size_t METADATA_MAX_LENGTH = 4080;
	const size_t METADATA_BLOCKSIZE_FACTOR = 16;
	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;
	const char* CRLF = "\r\n";

	InputData& inputData;
	TcpClient& tcpClient;
	Broadcaster* broadcaster;

	size_t dataChunkSize;

	bool readStatusLine();
	void readHeaders();
	void readData();

	void checkIfMetadataInterval(char* line);
	void checkIfRadioName(char* line);
	bool checkIfFinished();

	void readAudioBlock(char* audioBuffer);
	void readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer);

	void processAudio(char* audioBuffer, size_t dataSize);
	void processMetadata(char* metadataBuffer, size_t dataSize);

	static void convertHeaderNameToLowercase(char* line);
	static void printString(FILE* stream, char* string, size_t size);
};

#endif //RESPONSEPROCESSOR_HPP_
