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
};

#endif //RESPONSEPROCESSOR_HPP_
