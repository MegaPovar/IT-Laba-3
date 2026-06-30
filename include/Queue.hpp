#pragma once

#include <stdexcept>

#include "ListSequence.hpp"

template <class T>
class Queue {
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const {
        if (data_.GetLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }

public:
    Queue() = default;

    void Enqueue(const T& item) {
        data_.Append(item);
    }

    T Dequeue() {
        CheckNotEmpty();
        return data_.RemoveFirst();
    }

    T Peek() const {
        CheckNotEmpty();
        return data_.GetFirst();
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

    Queue<T> Concat(const Queue<T>& other) const {
        Queue<T> result(*this);
        for (int i = 0; i < other.GetCount(); ++i) {
            result.Enqueue(other.Get(i));
        }
        return result;
    }
};
