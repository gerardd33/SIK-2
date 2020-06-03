#ifndef ERRORHANDLER_HPP_
#define ERRORHANDLER_HPP_

#include <cstdio>
#include <cstdlib>

class ErrorHandler {
public:
	static void usage();
	static void error(const char* message);
};

#endif //ERRORHANDLER_HPP_
