MUDUO_DIRECTORY ?= $(HOME)/build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_base -lmuduo_net -lpthread -lrt

all: tcprelay socks4a balancer
clean:
	rm -f tcprelay core
	rm -f balancer core
	rm -f socks4a core

tcprelay: tcprelay.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

socks4a: socks4a.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

balancer: balancer.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: all clean
