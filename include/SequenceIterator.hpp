#pragma once

#include <stdexcept>
#include "Sequence.hpp"

template <class T>
class SequenceIterator {
private:
    const Sequence<T>* sequence;
    int position;

public:
    explicit SequenceIterator(const Sequence<T>& sequence) : sequence(&sequence), position(0) {}

    bool HasValue() const {
        return position < sequence->GetLength();
    }

    const T& Get() const {
        if (!HasValue()) {
            throw std::out_of_range("Iterator is out of range");
        }
        return sequence->Get(position);
    }

    void MoveNext() {
        if (HasValue()) {
            ++position;
        }
    }
};
