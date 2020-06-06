#include "Environment.hpp"

void signalHandler(int) {
	Environment::interrupted = 1;
}

namespace Environment {
	volatile std::sig_atomic_t interrupted = 0;

	void catchSigint() {
		struct sigaction signalAction{};
		signalAction.sa_flags = 0;
		signalAction.sa_handler = signalHandler;
		sigaction(SIGINT, &signalAction, nullptr);
	}
}
