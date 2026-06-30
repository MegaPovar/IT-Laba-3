#pragma once

#include <stdexcept>

#include "ListSequence.hpp"

template <class T>
class Deque {
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const {
        if (data_.GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
    }

public:
    Deque() = default;

    void PushFront(const T& item) {
        data_.Prepend(item);
    }

    void PushBack(const T& item) {
        data_.Append(item);
    }

    T PopFront() {
        CheckNotEmpty();
        return data_.RemoveFirst();
    }

    T PopBack() {
        CheckNotEmpty();
        return data_.RemoveLast();
    }

    const T& PeekFront() const {
        CheckNotEmpty();
        return data_.GetFirst();
    }

    const T& PeekBack() const {
        CheckNotEmpty();
        return data_.GetLast();
    }

    int GetCount() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.GetLength() == 0;
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Deque<T> result(*this);
        Deque<T> source(other);
        while (!source.IsEmpty()) {
            result.PushBack(source.PopFront());
        }
        return result;
    }
};
