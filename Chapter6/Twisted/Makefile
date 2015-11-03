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

all: finger01 finger02 finger03 finger04 finger05 finger06 finger07
clean:
	rm -f finger01 core
	rm -f finger02 core
	rm -f finger03 core
	rm -f finger04 core
	rm -f finger05 core
	rm -f finger06 core
	rm -f finger07 core

finger01: ../Echo/echo.cpp finger01.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger02: ../Echo/echo.cpp finger02.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger03: ../Echo/echo.cpp finger03.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger04: ../Echo/echo.cpp finger04.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger05: ../Echo/echo.cpp finger05.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger06: ../Echo/echo.cpp finger06.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

finger07: ../Echo/echo.cpp finger07.cpp
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
.PHONY: all clean
