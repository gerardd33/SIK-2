#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "InputData.hpp"
#include "TcpClient.hpp"
#include "UdpClient.hpp"
#include "RequestSender.hpp"
#include "ResponseProcessor.hpp"

class Player {
public:
	Player(int argc, const char** argv) : radioInfo(argc, argv) {}
	void run();

private:
	InputData radioInfo;
};

#endif //PLAYER_HPP_
