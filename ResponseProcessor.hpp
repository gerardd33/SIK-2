#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "ErrorHandler.hpp"
#include "RadioInfo.hpp"
#include <cctype>
#include <vector>

class ResponseProcessor {
public:
	explicit ResponseProcessor(RadioInfo& radioInfo, FILE* socketFile);
	void processServerResponse();

private:
	FILE* socketFile;
	int metadataInterval;
	bool requestMetadata;

	bool readStatusLine();
	void readHeaders();
	void readData();
	void checkIfMetadataInterval(char* line);
	void convertHeaderNameToLowercase(char* line);
	bool checkIfFinished();
	void readAudioBlock(char* audioBuffer);
	void readMetadataBlock(char* metadataSizeBuffer, char* metadataBuffer);

	const size_t METADATA_MAX_LENGTH = 4080;
	const size_t METADATA_BLOCKSIZE_FACTOR = 16;
	const size_t DEFAULT_CHUNK_SIZE = 8192;
};

#endif //RESPONSEPROCESSOR_HPP_
