MUDUO_DIRECTORY ?= $(HOME)/build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_base -lmuduo_net -lpthread -lrt

all: client server serverThreaded serverThreadedEfficient serverThreadedHighPerformance
clean:
	rm -f client core
	rm -f server core
	rm -f serverThreaded core
	rm -f serverThreadedEfficient core
	rm -f serverThreadedHighPerformance core


client: client.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

server: server.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

serverThreaded: serverThreaded.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

serverThreadedEfficient: serverThreadedEfficient.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

serverThreadedHighPerformance: serverThreadedHighPerformance.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
