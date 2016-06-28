
CC = gcc
CXX = g++

CXX_STD = -std=c++14
LDFLAGS = -pthread

GSL_LDFLAGS = -lgsl


test1: test1.cpp
	$(CXX) $(CXX_STD) $(LDFLAGS) -o $@ $@.cpp

test2: test2.cpp
	$(CXX) $(CXX_STD) $(LDFLAGS) -o $@ $@.cpp

test3: test3.cpp
	$(CXX) $(CXX_STD) $(LDFLAGS) $(GSL_LDFLAGS) -o $@ $@.cpp

test4: test4.cpp
	$(CXX) $(CXX_STD) $(LDFLAGS) $(GSL_LDFLAGS) -o $@ $@.cpp

clean:
	rm -f test1 test2 test3 test4

