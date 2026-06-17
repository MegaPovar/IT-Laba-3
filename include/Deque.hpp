#pragma once

#include <functional>

#include "LinearContainer.hpp"

template <class T>
class Deque : public LinearContainer<T> {
private:
    template <class>
    friend class Deque;

    explicit Deque(Sequence<T>* sequence, bool takeOwnership) : LinearContainer<T>(sequence, takeOwnership) {}

public:
    Deque() : LinearContainer<T>() {}
    explicit Deque(const Sequence<T>& sequence) : LinearContainer<T>(sequence) {}
    Deque(T* data, int count) : LinearContainer<T>(data, count) {}

    void PushFront(const T& value) {
        this->PrependFront(value);
    }

    void PushBack(const T& value) {
        this->AppendBack(value);
    }

    T PopFront() {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Deque is empty");
        }
        return this->RemoveFront();
    }

    T PopBack() {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Deque is empty");
        }
        return this->RemoveBack();
    }

    T PeekFront() const {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Deque is empty");
        }
        return this->items->GetFirst();
    }

    T PeekBack() const {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Deque is empty");
        }
        return this->items->GetLast();
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Sequence<T>* result = this->ConcatToSequence(other);
        return Deque<T>(result, true);
    }

    Deque<T> GetSubdeque(int startIndex, int endIndex) const {
        Sequence<T>* result = this->items->GetSubsequence(startIndex, endIndex);
        return Deque<T>(result, true);
    }

    template <class TResult>
    Deque<TResult> Map(std::function<TResult(T)> mapper) const {
        Sequence<TResult>* result = this->items->template Map<TResult>(mapper);
        return Deque<TResult>(result, true);
    }

    Deque<T> Where(std::function<bool(T)> predicate) const {
        Sequence<T>* result = this->items->Where(predicate);
        return Deque<T>(result, true);
    }

    Deque<T> operator+(const Deque<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Deque<T>& other) const {
        if (this->GetSize() != other.GetSize()) {
            return false;
        }
        for (int i = 0; i < this->GetSize(); ++i) {
            if (!(this->items->Get(i) == other.items->Get(i))) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Deque<T>& other) const {
        return !(*this == other);
    }
};
