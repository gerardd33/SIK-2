#ifndef ERRORHANDLER_HPP_
#define ERRORHANDLER_HPP_

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

class ErrorHandler {
public:
	static void usage();
	static void fatal(const char* message);
	static void syserr(const char* message);
	static void noexit(const char* message);

	// TODO usun wywolania
	static void debug(const char* message, int value = 0);
};

#endif //ERRORHANDLER_HPP_
