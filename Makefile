CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
GTEST_DIR = build/googletest
GTEST_INCLUDE = -I$(GTEST_DIR)/googletest/include -I$(GTEST_DIR)/googletest
GTEST_SRC = $(GTEST_DIR)/googletest/src/gtest-all.cc
GTEST_MAIN = $(GTEST_DIR)/googletest/src/gtest_main.cc

.PHONY: all test run clean

all: lab3 tests_runner

lab3: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o lab3

$(GTEST_SRC):
	mkdir -p build
	git clone --depth 1 --branch v1.14.0 https://github.com/google/googletest.git $(GTEST_DIR)

tests_runner: tests/tests.cpp $(GTEST_SRC)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) tests/tests.cpp $(GTEST_SRC) $(GTEST_MAIN) -pthread -o tests_runner

test: tests_runner
	./tests_runner

run: lab3
	./lab3

clean:
	rm -f lab3 tests_runner
