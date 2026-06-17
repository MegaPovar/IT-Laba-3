#pragma once

#include <functional>

#include "LinearContainer.hpp"

template <class T>
class Queue : public LinearContainer<T> {
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
        Queue<T> result(*this);
        typename LinearContainer<T>::Iterator iterator = other.Begin();
        while (iterator.HasValue()) {
            result.Enqueue(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    Queue<T> GetSubqueue(int startIndex, int endIndex) const {
        this->CheckSubsequenceIndexes(startIndex, endIndex);
        Queue<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        int position = 0;
        while (iterator.HasValue()) {
            if (position >= startIndex && position <= endIndex) {
                result.Enqueue(iterator.Get());
            }
            iterator.MoveNext();
            ++position;
        }
        return result;
    }

    template <class TResult>
    Queue<TResult> Map(std::function<TResult(T)> mapper) const {
        Queue<TResult> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            result.Enqueue(mapper(iterator.Get()));
            iterator.MoveNext();
        }
        return result;
    }

    Queue<T> Where(std::function<bool(T)> predicate) const {
        Queue<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            T value = iterator.Get();
            if (predicate(value)) {
                result.Enqueue(value);
            }
            iterator.MoveNext();
        }
        return result;
    }

    Queue<T> operator+(const Queue<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Queue<T>& other) const {
        return this->HasSameItems(other);
    }

    bool operator!=(const Queue<T>& other) const {
        return !(*this == other);
    }
};
