cmake_minimum_required(VERSION 3.15)
project(SIK_2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(radio-proxy main.cpp Player.cpp Player.hpp InputData.hpp InputData.cpp TcpClient.cpp TcpClient.hpp ErrorHandler.cpp ErrorHandler.hpp RequestSender.cpp RequestSender.hpp ResponseProcessor.cpp ResponseProcessor.hpp UdpClient.cpp UdpClient.hpp Broadcaster.cpp Broadcaster.hpp LastContactStorage.cpp LastContactStorage.hpp Environment.cpp Environment.hpp)
