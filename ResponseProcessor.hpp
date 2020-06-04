#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "ErrorHandler.hpp"
#include "RadioInfo.hpp"
#include <cctype>
#include <vector>

class ResponseProcessor {
public:
	ResponseProcessor(RadioInfo& radioInfo, FILE* socketFile);
	void processServerResponse();

private:
	FILE* socketFile;
	size_t dataChunkSize;
	bool requestMetadata;

	const size_t METADATA_MAX_LENGTH = 4080;
	const size_t METADATA_BLOCKSIZE_FACTOR = 16;
	const size_t DEFAULT_DATA_CHUNK_SIZE = 8192;

	bool readStatusLine();
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
