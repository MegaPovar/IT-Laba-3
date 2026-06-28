#pragma once

#include <string>

#include <stdexcept>

inline int SquareTestInt(int value) {
    return value * value;
}

inline bool IsEvenTestInt(int value) {
    return value % 2 == 0;
}

inline int SumTestInt(int sum, int value) {
    return sum + value;
}

inline int AddIndexTestInt(int value, int index) {
    return value + index;
}

inline int DoubleTestInt(int value) {
    return value * 2;
}

inline bool AtLeastTwenty(int value) {
    return value >= 20;
}

inline bool IsLongWord(std::string value) {
    return value.size() > 3;
}

inline int StringLength(std::string value) {
    return static_cast<int>(value.size());
}
