#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "ErrorHandler.hpp"
#include "InputData.hpp"
#include <cctype>
#include <vector>

class ResponseProcessor {
public:
	ResponseProcessor(InputData& radioInfo, FILE* radioSocketFile, FILE* broadcastSocketFile);
	void processServerResponse();

private:
	FILE* radioSocketFile;
	FILE* broadcastSocketFile;
	size_t dataChunkSize;
	bool requestMetadata;

	const size_t METADATA_MAX_LENGTH = 4080;
	const size_t METADATA_BLOCKSIZE_FACTOR = 16;
	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;

	void processAudio(char* audioBuffer, size_t dataSize);
	void processMetadata(char* metadataBuffer, size_t dataSize);
	void readStatusLine();
	void readHeaders();
	void readData();
	void checkIfMetadataInterval(char* line);
	bool checkIfFinished();
	void readAudioBlock(char* audioBuffer);
	void readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer);
	static void convertHeaderNameToLowercase(char* line);
	static void printString(FILE* stream, char* string, size_t size);
};

#endif //RESPONSEPROCESSOR_HPP_
