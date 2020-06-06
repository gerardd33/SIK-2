#ifndef INTERRUPTEDEXCEPTION_HPP_
#define INTERRUPTEDEXCEPTION_HPP_

#include <exception>

class InterruptedException : public std::exception {
	const char* what() const noexcept override {
		return "Interrupted";
	}
};

#endif //INTERRUPTEDEXCEPTION_HPP_
