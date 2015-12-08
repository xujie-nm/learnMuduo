LIB_SRC = Channel.cpp EventLoop.cpp Poller.cpp
BINARIES = test1 test2 test2_1 test3

all: $(BINARIES)

include ../reactor.mk

test1: test1.cpp
test2: test2.cpp
test2_1: test2_1.cpp
test3: test3.cpp

.PHONY: all clean
