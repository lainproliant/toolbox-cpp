CXXFLAGS=-g --std=c++11 -DLAIN_OPTARG_DEBUG -I../include
LDFLAGS=
LDLIBS=

SRCS=getopt_test.cpp
BINS=$(subst .cpp,,$(SRCS))

getopt_test: getopt_test.cpp
	g++ $(CXXFLAGS) getopt_test.cpp -o getopt_test

clean:
	rm -f $(BINS)

