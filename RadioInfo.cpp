#include "RadioInfo.hpp"

RadioInfo::RadioInfo(int argc, const char** argv) {
	// Default values of optional arguments.
	this->timeout = DEFAULT_TIMEOUT;
	this->requestMetadata = false;

	this->radioHost = nullptr;
	this->radioResourcePath = nullptr;
	this->radioPort = -1;

	parseArguments(argc, argv);

	// Any of the required arguments is absent.
	if (this->radioHost == nullptr || this->radioResourcePath == nullptr || this->radioPort == -1) {
		usage();
	}
}

void RadioInfo::usage() {
	printf("Usage: ./radio-proxy -h host -r resource -p port [-m yes|no] [-t timeout]\n");
	exit(1);
}

void RadioInfo::parseArguments(int argc, const char** argv) {
	for (int argId = 1; argId < argc; ++argId) {
		printf("%d %d %s\n", argc, argId, argv[argId]);
		if (argId % 2 == 1) { // A flag identifying the argument.
			if (argId + 1 >= argc) { // No value for this argument.
				usage();
			}

			char *argument = nullptr;
			int sscanfResult = sscanf(argv[argId], "-%ms", &argument);
			if (sscanfResult == 0 || argument[1] != 0) {
				usage();
			}

			const char *argValue = argv[argId + 1];
			switch (argument[0]) {
				case 'h': this->radioHost = argValue;
					break;

				case 'r': this->radioResourcePath = argValue;
					break;

				case 'p': this->radioPort = static_cast<int>(strtol(argValue, nullptr, 10));
					if (errno != 0 || this->radioPort < 0) { // Invalid number.
						usage();
					}
					break;
				case 'm':
					if (strcmp(argValue, "yes") == 0) {
						this->requestMetadata = true;
					} else if (strcmp(argValue, "no") == 0) {
						this->requestMetadata = false;
					} else {
						usage();
					}
					break;

				case 't':
					this->timeout = static_cast<int>(strtol(argValue, nullptr, 10));
					if (errno != 0 || this->timeout <= 0) { // Invalid number.
						usage();
					}
					break;

				default:
					usage();
			}
		}
	}
}
