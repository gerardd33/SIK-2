#ifndef ERRORHANDLER_HPP_
#define ERRORHANDLER_HPP_

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Environment.hpp"

class ErrorHandler {
public:
	static void usage() __attribute__((noreturn));
	static void fatal(const char* message) __attribute__((noreturn));
	static void syserr(const char* message) __attribute__((noreturn));
	static void checkInterrupted();
};

#endif  // ERRORHANDLER_HPP_
