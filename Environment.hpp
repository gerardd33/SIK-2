#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include <atomic>
#include <csignal>
#include <exception>

namespace Environment {
	extern volatile std::sig_atomic_t interrupted;
	void catchSigint();

	class InterruptedException : public std::exception {
		const char* what() const noexcept override {
			return "Interrupted";
		}
	};
}  // namespace Environment

#endif  // ENVIRONMENT_HPP_
