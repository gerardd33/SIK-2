#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "RadioInfo.hpp"
#include "TcpClient.hpp"
#include "RequestSender.hpp"
#include "ResponseProcessor.hpp"

class Player {
public:
	Player(int argc, const char** argv) : radioInfo(argc, argv) {};
	void run();

private:
	RadioInfo radioInfo;
};

#endif //PLAYER_HPP_
