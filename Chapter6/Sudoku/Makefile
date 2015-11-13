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

all: server_basic server_multiloop server_threadpool
clean:
	rm -f server_basic core
	rm -f server_multiloop core
	rm -f server_threadpool core

server_basic: sudoku.cpp server_basic.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

server_multiloop: sudoku.cpp server_multiloop.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

server_threadpool: sudoku.cpp server_threadpool.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
