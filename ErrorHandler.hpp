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
};

#endif //ERRORHANDLER_HPP_
