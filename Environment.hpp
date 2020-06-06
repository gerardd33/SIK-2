#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include <csignal>
#include <atomic>
#include <exception>

namespace Environment {
	extern volatile std::sig_atomic_t interrupted;
	void catchSigint();

	class InterruptedException : public std::exception {
		const char* what() const noexcept override {
			return "Interrupted";
		}
	};
}

#endif //ENVIRONMENT_HPP_
