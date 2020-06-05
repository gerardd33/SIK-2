#include "Player.hpp"
#include <iostream>

int main(int argc, const char** argv) {
	std::cerr << "Started running." << std::endl;

	Player player(argc, argv);
	player.run();

	std::cerr << "Finished running." << std::endl;
	return 0;
}