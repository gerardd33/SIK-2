#include "ErrorHandler.hpp"

void ErrorHandler::usage() {
	fprintf(stderr, "Usage: ./radio-proxy -h host -r resource -p port [-m yes|no] [-t timeout]\n");
	exit(1);
}

void ErrorHandler::fatal(const char* message) {
	fprintf(stderr, "ERROR: %s\n", message);
	exit(1);
}

void ErrorHandler::syserr(const char* message) {
	fprintf(stderr, "ERROR: %s\n", message);
	fprintf(stderr, "(%d; %s)\n", errno, strerror(errno));
	exit(1);
}