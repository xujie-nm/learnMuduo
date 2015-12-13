LIB_SRC = Channel.cpp EventLoop.cpp EventLoopThread.cpp Poller.cpp Timer.cpp TimerQueue.cpp
BINARIES = test1 test2 test2_1 test3 test4 test5 test6

all: $(BINARIES)

include ../reactor.mk

test1: test1.cpp
test2: test2.cpp
test2_1: test2_1.cpp
test3: test3.cpp
test4: test4.cpp
test5: test5.cpp
test6: test6.cpp

.PHONY: all clean
