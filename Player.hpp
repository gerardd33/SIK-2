#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Broadcaster.hpp"
#include "InputData.hpp"
#include "RequestSender.hpp"
#include "ResponseProcessor.hpp"
#include "TcpClient.hpp"

class Player {
public:
	Player(int argc, const char** argv) : inputData(argc, argv) {
	}
	void run();

private:
	InputData inputData;
};

#endif  // PLAYER_HPP_
