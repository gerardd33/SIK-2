#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "RadioInfo.hpp"
#include "TcpClient.hpp"

class Player {
public:
	Player(int argc, const char** argv);
	void run();
};

#endif //PLAYER_HPP_
