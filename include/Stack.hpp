#pragma once

#include <stdexcept>

#include "ListSequence.hpp"

template <class T>
class Stack {
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const {
        if (data_.GetLength() == 0) {
            throw std::out_of_range("Stack is empty");
        }
    }

public:
    Stack() = default;

    void Push(const T& item) {
        data_.Prepend(item);
    }

    T Pop() {
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

    Stack<T> Concat(const Stack<T>& other) const {
        Stack<T> result(*this);
        for (int i = other.GetCount() - 1; i >= 0; --i) {
            result.Push(other.Get(i));
        }
        return result;
    }
};
