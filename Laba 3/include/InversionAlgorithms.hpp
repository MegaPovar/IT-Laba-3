#pragma once

#include <functional>

#include "SequenceAlgorithms.hpp"

template <class T>
long long CountInversionsLoops(const Sequence<T>& sequence, std::function<bool(T, T)> less) {
    long long count = 0;
    for (int i = 0; i < sequence.GetLength(); ++i) {
        for (int j = i + 1; j < sequence.GetLength(); ++j) {
            if (less(sequence.Get(j), sequence.Get(i))) {
                ++count;
            }
        }
    }
    return count;
}

template <class T>
long long CountInversionsMapReduce(const Sequence<T>& sequence, std::function<bool(T, T)> less) {
    Sequence<long long>* counts = sequence.template MapIndexed<long long>(
        [&sequence, less](T value, int index) {
            long long count = 0;
            for (int j = index + 1; j < sequence.GetLength(); ++j) {
                if (less(sequence.Get(j), value)) {
                    ++count;
                }
            }
            return count;
        });

    long long result = counts->template Reduce<long long>(
        [](long long sum, long long value) { return sum + value; }, 0);
    delete counts;
    return result;
}
