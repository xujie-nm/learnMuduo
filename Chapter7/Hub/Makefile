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

all: hub pub sub
clean:
	rm -f hub core
	rm -f pub core
	rm -f sub core


hub: codec.cpp hub.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

pub: codec.cpp pubsub.cpp pub.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sub: codec.cpp pubsub.cpp sub.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
