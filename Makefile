CXX      := clang++
CXXOPT   := -O3
CXXFLAGS := -std=c++23 -Wall -Wextra -Iinclude 

all:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(CXXOPT) -o bin/8x8 8x8.cpp
	$(CXX) $(CXXFLAGS) $(CXXOPT) -o bin/nxn nxn.cpp
