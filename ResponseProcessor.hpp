#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "ErrorHandler.hpp"
#include <cctype>
#include <vector>

class ResponseProcessor {
public:
	explicit ResponseProcessor(FILE* socketFile) : socketFile(socketFile) {};
	void processServerResponse();

private:
	FILE* socketFile;
	int metadataInterval = -1;

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
};

#endif //RESPONSEPROCESSOR_HPP_
