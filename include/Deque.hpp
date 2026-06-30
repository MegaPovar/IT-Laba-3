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

    T PeekFront() const {
        CheckNotEmpty();
        return data_.GetFirst();
    }

    T PeekBack() const {
        CheckNotEmpty();
        return data_.GetLast();
    }

    int GetCount() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.GetLength() == 0;
    }

    T Get(int index) const {
        return data_.Get(index);
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Deque<T> result(*this);
        for (int i = 0; i < other.GetCount(); ++i) {
            result.PushBack(other.Get(i));
        }
        return result;
    }
};
