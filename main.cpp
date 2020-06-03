#include <iostream>
#include "Player.hpp"

int main(int argc, const char** argv) {
	std::cout << "Started running." << std::endl;

	Player player(argc, argv);
	player.run();

	std::cout << "Finished running." << std::endl;
	return 0;
}