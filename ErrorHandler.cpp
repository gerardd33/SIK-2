#include "ErrorHandler.hpp"

void ErrorHandler::checkInterrupted() {
	if (errno == EINTR || Environment::interrupted) {
		throw InterruptedException();
	}
}

void ErrorHandler::usage() {
	checkInterrupted();

	fprintf(stderr, "Usage: ./radio-proxy -h radio_host -r radio_resource -p radio_port "
					"[-m yes|no] [-t radio_timeout] [-P broadcast_port "
					"[-B broadcast_multicast_address] [-T broadcast_timeout]]\n");
	exit(1);
}

void ErrorHandler::fatal(const char* message) {
	checkInterrupted();

	fprintf(stderr, "ERROR: %s\n", message);
	exit(1);
}

void ErrorHandler::syserr(const char* message) {
	checkInterrupted();

	fprintf(stderr, "ERROR: %s\n", message);
	fprintf(stderr, "(%d; %s)\n", errno, strerror(errno));
	exit(1);
}

void ErrorHandler::noexit(const char* message) {
	checkInterrupted();

	fprintf(stderr, "ERROR: %s\n", message);
}

// TODO usun
void ErrorHandler::debug(const char* message, int value) {
	fprintf(stderr, "DEBUG: %s: %d\n", message, value);
}