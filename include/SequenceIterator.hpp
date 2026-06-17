#pragma once

#include "Exceptions.hpp"
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

    T Get() const {
        if (!HasValue()) {
            throw IndexOutOfRange("Iterator is out of range");
        }
        return sequence->Get(position);
    }

    void MoveNext() {
        if (HasValue()) {
            ++position;
        }
    }
};
