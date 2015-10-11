CXXFLAGS = -g -O0 -Wall -Wextra -Werror\
		   -Wconversion -Wno-unused-parameter\
		   -Wold-style-cast -Woverloaded-virtual\
		   -Wpointer-arith -Wshadow -Wwrite-strings\

LDFLAGS = ../Thread.cpp -lpthread -lrt

all: Factory
clean:
	rm -f Factory core

Observer: Factory.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
