CXXFLAGS = -g -O0 -Wall -Wextra -Werror\
		   -Wconversion -Wno-unused-parameter\
		   -Wold-style-cast -Woverloaded-virtual\
		   -Wpointer-arith -Wshadow -Wwrite-strings\

LDFLAGS = ../Thread.cpp -lpthread -lrt

all: NonRecursiveMutex
clean:
	rm -f NonRecursiveMutex core

Factory: NonRecursiveMutex.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
