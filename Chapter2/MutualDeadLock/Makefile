CXXFLAGS = -g -O0 -Wall -Wextra -Werror\
		   -Wconversion -Wno-unused-parameter\
		   -Wold-style-cast -Woverloaded-virtual\
		   -Wpointer-arith -Wshadow -Wwrite-strings\

LDFLAGS = ../Thread.cpp -lpthread -lrt

all: MutualDeadLock AnotherMutualDeadLock
clean:
	rm -f MutualDeadLock core
	rm -f AnotherMutualDeadLock core

MutualDeadLock: MutualDeadLock.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

AnotherMutualDeadLock: AnotherMutualDeadLock.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
