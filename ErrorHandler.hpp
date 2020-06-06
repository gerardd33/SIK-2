#ifndef ERRORHANDLER_HPP_
#define ERRORHANDLER_HPP_

#include "Environment.hpp"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

class ErrorHandler {
public:
	static void usage() __attribute__((noreturn));
	static void fatal(const char* message) __attribute__((noreturn));
	static void syserr(const char* message) __attribute__((noreturn));
	static void checkInterrupted();
};

#endif //ERRORHANDLER_HPP_
