#pragma once

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "SequenceAlgorithms.hpp"

inline Sequence<int>* Range(int low, int high) {
    if (low > high) {
        throw InvalidArgument("Range low cannot be greater than high");
    }

    Sequence<int>* result = new MutableArraySequence<int>();
    for (int value = low; value <= high; ++value) {
        result->Append(value);
    }
    return result;
}

inline bool IsPrime(int value) {
    if (value < 2) {
        return false;
    }
    for (int divisor = 2; divisor <= value / divisor; ++divisor) {
        if (value % divisor == 0) {
            return false;
        }
    }
    return true;
}

inline Sequence<int>* PrimesInRange(int low, int high) {
    Sequence<int>* numbers = Range(low, high);
    Sequence<int>* primes = numbers->Where([](int value) { return IsPrime(value); });
    delete numbers;
    return primes;
}

inline Sequence<int>* Factorize(int value) {
    if (value <= 0) {
        throw InvalidArgument("Factorized value must be positive");
    }

    Sequence<int>* factors = new MutableArraySequence<int>();
    for (int divisor = 1; divisor <= value / divisor; ++divisor) {
        if (value % divisor == 0) {
            factors->Append(divisor);
            if (divisor != value / divisor) {
                factors->Append(value / divisor);
            }
        }
    }
    return factors;
}

inline Sequence<int>* PrimeFactorization(int value) {
    if (value <= 1) {
        throw InvalidArgument("Value for prime factorization must be greater than 1");
    }

    Sequence<int>* factors = new MutableArraySequence<int>();
    int current = value;
    for (int divisor = 2; divisor <= current / divisor; ++divisor) {
        while (current % divisor == 0) {
            factors->Append(divisor);
            current /= divisor;
        }
    }
    if (current > 1) {
        factors->Append(current);
    }
    return factors;
}
