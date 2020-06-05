#include <cstdlib>
#include "RadioInfo.hpp"

RadioInfo::RadioInfo(int argc, const char** argv) {
	// Default values of optional arguments.
	this->radioTimeout = DEFAULT_RADIO_TIMEOUT;
	this->requestMetadata = false;

	this->radioHost = nullptr;
	this->radioResourcePath = nullptr;
	this->radioPort = nullptr;

	parseArguments(argc, argv);

	// Any of the required arguments is absent.
	if (this->radioHost == nullptr || this->radioResourcePath == nullptr || this->radioPort == nullptr) {
		ErrorHandler::usage();
	}
}

void RadioInfo::assignArgument(char argumentFlag, const char* argumentValue) {
	switch (argumentFlag) {
		case 'h':
			this->radioHost = strdup(argumentValue);
			break;

		case 'r':
			this->radioResourcePath = strdup(argumentValue);
			break;

		case 'p':
			this->radioPort = strdup(argumentValue);
			break;

		case 'm':
			if (strcmp(argumentValue, "yes") == 0) {
				this->requestMetadata = true;
			} else if (strcmp(argumentValue, "no") == 0) {
				this->requestMetadata = false;
			} else {
				ErrorHandler::usage();
			}
			break;

		case 't':
			this->radioTimeout = static_cast<int>(strtol(argumentValue, nullptr, 10));
			if (errno != 0 || this->radioTimeout <= 0) { // Invalid number.
				ErrorHandler::usage();
			}
			break;

		case 'P':
			this->broadcastPort = strdup(argumentValue);
			break;

		case 'B':
			this->broadcastMulticastAddress = strdup(argumentValue);

		case 'T':
			this->broadcastTimeout = static_cast<int>(strtol(argumentValue, nullptr, 10));
			if (errno != 0 || this->broadcastTimeout <= 0) { // Invalid number.
				ErrorHandler::usage();
			}
			break;

		default:
			ErrorHandler::usage();
	}
}

void RadioInfo::parseArguments(int argc, const char** argv) {
	for (int argumentId = 1; argumentId < argc; ++argumentId) {
		if (argumentId % 2 == 1) { // A flag identifying the argument.
			if (argumentId + 1 >= argc) { // No value for this argument.
				ErrorHandler::usage();
			}

			char* argumentFlag = nullptr;
			int sscanfResult = sscanf(argv[argumentId], "-%ms", &argumentFlag);
			if (sscanfResult == 0 || argumentFlag[1] != 0) {
				ErrorHandler::usage();
			}

			assignArgument(argumentFlag[0], argv[argumentId + 1]);
		} else if (argv[argumentId][0] == '-') { // A value of the argument.
			ErrorHandler::usage();
		}
	}
}

RadioInfo::~RadioInfo() {
	free((char*) radioHost);
	free((char*) radioResourcePath);
	free((char*) radioPort);
}
