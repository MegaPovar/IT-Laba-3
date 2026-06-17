#pragma once

#include <functional> // для сравнения std::function<bool(T, T)>

#include "SequenceAlgorithms.hpp"

inline long long SumInversionCounts(long long sum, long long value) {
    return sum + value;
}

template <class T>
class InversionCounter {
private:
    const Sequence<T>& sequence;
    std::function<bool(T, T)> less;

public:
    InversionCounter(const Sequence<T>& sequence, std::function<bool(T, T)> less)
        : sequence(sequence), less(less) {}

    long long operator()(T value, int index) const {
        long long count = 0;
        for (int j = index + 1; j < sequence.GetLength(); ++j) {
            if (less(sequence.Get(j), value)) { // если элемент справа меньше текущего, то это инверсия
                ++count;
            }
        }
        return count;
    }
};

template <class T> // [3, 1, 2] -> 2 инв
long long CountInversionsLoops(const Sequence<T>& sequence, std::function<bool(T, T)> less) { // подсчет двумя циклами
    long long count = 0;
    for (int i = 0; i < sequence.GetLength(); ++i) {
        for (int j = i + 1; j < sequence.GetLength(); ++j) {
            if (less(sequence.Get(j), sequence.Get(i))) { // само сравнение
                ++count;
            }
        }
    }
    return count;
}

template <class T> // [3, 1, 2] -> [2, 0, 0]
long long CountInversionsMapReduce(const Sequence<T>& sequence, std::function<bool(T, T)> less) { // вариант через MapIndexed и Reduce
    Sequence<long long>* counts = sequence.template MapIndexed<long long>(
        InversionCounter<T>(sequence, less));

    long long result = counts->template Reduce<long long>(
        SumInversionCounts, 0); // [2, 0, 0] -> 2
    delete counts;
    return result;
}
