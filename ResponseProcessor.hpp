#ifndef RESPONSEPROCESSOR_HPP_
#define RESPONSEPROCESSOR_HPP_

#include "ErrorHandler.hpp"

class ResponseProcessor {
public:
	explicit ResponseProcessor(FILE* socketFile) : socketFile(socketFile) {};

private:
	FILE* socketFile;

	bool readStatusLine();
};

#endif //RESPONSEPROCESSOR_HPP_
