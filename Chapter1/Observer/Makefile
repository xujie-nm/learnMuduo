MUDUO_DIRECTORY ?= $(HOME)/build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -g -O0 -Wall -Wextra -Werror\
		   -Wconversion -Wno-unused-parameter\
		   -Wold-style-cast -Woverloaded-virtual\
		   -Wpointer-arith -Wshadow -Wwrite-strings\

LDFLAGS = -I$(MUDUO_INCLUDE) -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt

all: Observer
clean:
	rm -f Observer core

Counter: Observer.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
