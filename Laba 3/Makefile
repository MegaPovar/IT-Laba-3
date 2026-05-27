CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

.PHONY: all test run clean

all: lab3 tests_runner

lab3: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o lab3

tests_runner: tests/tests.cpp
	$(CXX) $(CXXFLAGS) tests/tests.cpp -o tests_runner

test: tests_runner
	./tests_runner

run: lab3
	./lab3

clean:
	rm -f lab3 tests_runner
