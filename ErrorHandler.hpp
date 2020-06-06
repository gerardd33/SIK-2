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
	static void noexit(const char* message);
	static void checkInterrupted();

	// TODO usun wywolania
	static void debug(const char* message, int value = 0);
};

#endif //ERRORHANDLER_HPP_
