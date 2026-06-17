#pragma once

#include <functional>

#include "LinearContainer.hpp"

template <class T>
class Queue : public LinearContainer<T> {
private:
    template <class>
    friend class Queue;

    explicit Queue(Sequence<T>* sequence, bool takeOwnership) : LinearContainer<T>(sequence, takeOwnership) {}

public:
    Queue() : LinearContainer<T>() {}
    explicit Queue(const Sequence<T>& sequence) : LinearContainer<T>(sequence) {}
    Queue(T* data, int count) : LinearContainer<T>(data, count) {}

    void Enqueue(const T& value) {
        this->AppendBack(value);
    }

    T Dequeue() {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Queue is empty");
        }
        return this->RemoveFront();
    }

    T Peek() const {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Queue is empty");
        }
        return this->items->GetFirst();
    }

    Queue<T> Concat(const Queue<T>& other) const {
        Sequence<T>* result = this->ConcatToSequence(other);
        return Queue<T>(result, true);
    }

    Queue<T> GetSubqueue(int startIndex, int endIndex) const {
        Sequence<T>* result = this->items->GetSubsequence(startIndex, endIndex);
        return Queue<T>(result, true);
    }

    template <class TResult>
    Queue<TResult> Map(std::function<TResult(T)> mapper) const {
        Sequence<TResult>* result = this->items->template Map<TResult>(mapper);
        return Queue<TResult>(result, true);
    }

    Queue<T> Where(std::function<bool(T)> predicate) const {
        Sequence<T>* result = this->items->Where(predicate);
        return Queue<T>(result, true);
    }

    Queue<T> operator+(const Queue<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Queue<T>& other) const {
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

    bool operator!=(const Queue<T>& other) const {
        return !(*this == other);
    }
};
