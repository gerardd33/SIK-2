#include "Environment.hpp"
#include "Player.hpp"

int main(int argc, const char** argv) {
	Environment::catchSigint();

	Player player(argc, argv);
	try {
		player.run();
	} catch (...) {
		fprintf(stderr, "CAUGHT\n");
	}
	return 0;
}