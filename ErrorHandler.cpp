#include "ErrorHandler.hpp"

void ErrorHandler::usage() {
	fprintf(stderr, "Usage: ./radio-proxy -h radio_host -r radio_resource -p radio_port "
						 "[-m yes|no] [-t radio_timeout] -P broadcast_port "
					     "[-B broadcast_multicast_address] [-T broadcast_timeout] \n");
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