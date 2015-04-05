CXX=g++
CXXFLAGS=-g --std=c++14 -DLAIN_OPTARG_DEBUG -I../include
LDFLAGS=
LDLIBS=

all: unit-test clean

unit-test: build-all-tests
	./run-tests.py

build-all-tests: $(patsubst %.cpp, %.test, $(wildcard *.cpp))

%.test: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.test

