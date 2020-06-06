GCC = g++
CPPFLAGS = -Wall -Wextra -O2 -pthread

.PHONY: all clean

all: radio-proxy

radio-proxy: main.o Environment.o InputData.o Player.o TcpClient.o RequestSender.o ResponseProcessor.o Broadcaster.o UdpClient.o LastContactStorage.o ErrorHandler.o
	$(GCC) $(CPPFLAGS) -o radio-proxy $^

main.o: main.cpp Environment.hpp Player.hpp
	$(GCC) $(CPPFLAGS) -c $<

Environment.o: Environment.cpp Environment.hpp
	$(GCC) $(CPPFLAGS) -c $<

ErrorHandler.o: ErrorHandler.cpp ErrorHandler.hpp Environment.hpp
	$(GCC) $(CPPFLAGS) -c $<

InputData.o: InputData.cpp InputData.hpp ErrorHandler.hpp
	$(GCC) $(CPPFLAGS) -c $<

TcpClient.o: TcpClient.cpp TcpClient.hpp InputData.hpp
	$(GCC) $(CPPFLAGS) -c $<

Player.o: Player.cpp Player.hpp InputData.hpp TcpClient.hpp RequestSender.hpp ResponseProcessor.hpp Broadcaster.hpp
	$(GCC) $(CPPFLAGS) -c $<

RequestSender.o: RequestSender.cpp RequestSender.hpp InputData.hpp TcpClient.hpp
	$(GCC) $(CPPFLAGS) -c $<

ResponseProcessor.o: ResponseProcessor.cpp ResponseProcessor.hpp Environment.hpp ErrorHandler.hpp InputData.hpp Broadcaster.hpp TcpClient.hpp
	$(GCC) $(CPPFLAGS) -c $<

Broadcaster.o: Broadcaster.cpp Broadcaster.hpp Environment.hpp UdpClient.hpp LastContactStorage.hpp
	$(GCC) $(CPPFLAGS) -c $<

UdpClient.o: UdpClient.cpp UdpClient.hpp InputData.hpp
	$(GCC) $(CPPFLAGS) -c $<

LastContactStorage.o: LastContactStorage.cpp LastContactStorage.hpp InputData.hpp
	$(GCC) $(CPPFLAGS) -c $<

clean:
	rm -f *.o radio-proxy
