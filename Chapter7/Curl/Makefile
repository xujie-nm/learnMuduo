MUDUO_DIRECTORY ?= $(HOME)/build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_base -lmuduo_net -lpthread -lrt -lcurl

all: download mcurl
clean:
	rm -f download core
	rm -f mcurl core

download: curl.cpp download.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mcurl: curl.cpp mcurl.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
