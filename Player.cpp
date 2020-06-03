#include "Player.hpp"
#include "RadioInfo.hpp"

Player::Player(int argc, const char** argv) {
	RadioInfo radioInfo(argc, argv);

	printf("Host: %s:\n", radioInfo.getRadioHost());
	printf("Resource: %s:\n", radioInfo.getRadioResourcePath());
	printf("Port: %d\n", radioInfo.getRadioPort());
	printf("Timeout: %d\n", radioInfo.getTimeout());
	printf("Request metadata?: %d\n", radioInfo.isRequestMetadata());
}

void Player::run() {

}
