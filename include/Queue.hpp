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

    const T& Peek() const {
        CheckNotEmpty();
        return data_.GetFirst();
    }

    int GetCount() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.GetLength() == 0;
    }

    Queue<T> Concat(const Queue<T>& other) const {
        Queue<T> result(*this);
        Queue<T> source(other);
        while (!source.IsEmpty()) {
            result.Enqueue(source.Dequeue());
        }
        return result;
    }
};
