#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include <csignal>
#include <atomic>

namespace Environment {
	extern volatile std::sig_atomic_t interrupted;
	void catchSigint();
}

#endif //ENVIRONMENT_HPP_
