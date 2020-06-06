#include <cstdlib>
#include "InputData.hpp"

InputData::InputData(int argc, const char** argv) : requestMetadata(false), radioHost(nullptr),
	radioResourcePath(nullptr), radioPort(nullptr), radioTimeout(DEFAULT_TIMEOUT), broadcasting(false),
	multicast(false), broadcastMulticastAddress(nullptr), broadcastPort(0), broadcastTimeout(DEFAULT_TIMEOUT) {
	parseArguments(argc, argv);

	// Any of the required arguments is absent.
	if (this->radioHost == nullptr || this->radioResourcePath == nullptr || this->radioPort == nullptr) {
		ErrorHandler::usage();
	}
}

InputData::~InputData() {
	free((char*) radioHost);
	free((char*) radioResourcePath);
	free((char*) radioPort);

	if (this->broadcastMulticastAddress != nullptr) {
		free((char*) this->broadcastMulticastAddress);
	}
}

void InputData::parseArguments(int argc, const char** argv) {
	for (int argumentId = 1; argumentId < argc; ++argumentId) {
		if (argumentId % 2 == 1) { // A flag identifying the argument.
			if (argumentId + 1 >= argc) { // No value entered for this argument.
				ErrorHandler::usage();
			}

			char* argumentFlag = nullptr;
			int itemsRead = sscanf(argv[argumentId], "-%ms", &argumentFlag);
			if (itemsRead == 0 || argumentFlag[1] != 0) {
				ErrorHandler::usage();
			}

			assignArgument(argumentFlag[0], argv[argumentId + 1]);
			free(argumentFlag);
		} else if (argv[argumentId][0] == '-') { // A value of the argument.
			ErrorHandler::usage();
		}
	}
}

void InputData::assignArgument(char argumentFlag, const char* argumentValue) {
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
			this->radioTimeout = parseInteger(argumentValue);
			if (this->radioTimeout <= 0) {
				ErrorHandler::usage();
			}
			break;

		case 'P':
			this->broadcasting = true;
			this->broadcastPort = parseInteger(argumentValue);
			if (this->broadcastPort < 0) {
				ErrorHandler::usage();
			}
			break;

		case 'B':
			this->multicast = true;
			this->broadcastMulticastAddress = strdup(argumentValue);
			break;

		case 'T':
			this->broadcastTimeout = parseInteger(argumentValue);
			if (this->broadcastTimeout <= 0) {
				ErrorHandler::usage();
			}
			break;

		default:
			ErrorHandler::usage();
	}
}

int InputData::parseInteger(const char* string) {
	int parsedInteger = static_cast<int>(strtol(string, nullptr, 10));
	if (parsedInteger == 0 && errno != 0) { // Invalid number.
		ErrorHandler::usage();
	}

	return parsedInteger;
}
