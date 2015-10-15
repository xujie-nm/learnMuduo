CXXFLAGS = -g -O0 -Wall -Wextra -Werror\
		   -Wconversion -Wno-unused-parameter\
		   -Wold-style-cast -Woverloaded-virtual\
		   -Wpointer-arith -Wshadow -Wwrite-strings\

LDFLAGS = ../Thread.cpp -lpthread -lrt -std=c++0x

all: SignalSlotTest
clean:
	rm -f SignalSlotTest core

SignalSlotTest: SignalSlotTest.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
