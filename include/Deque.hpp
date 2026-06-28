#pragma once

#include "LinearContainer.hpp"
#include <stdexcept>

template <class T>
class Deque : public LinearContainer<T> {
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
            throw std::out_of_range("Deque is empty");
        }
        return this->RemoveFront();
    }

    T PopBack() {
        if (this->IsEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        return this->RemoveBack();
    }

    T PeekFront() const {
        if (this->IsEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        return this->items->GetFirst();
    }

    T PeekBack() const {
        if (this->IsEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        return this->items->GetLast();
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Deque<T> result(*this);
        typename LinearContainer<T>::Iterator iterator = other.Begin();
        while (iterator.HasValue()) {
            result.PushBack(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    Deque<T> GetSubdeque(int startIndex, int endIndex) const {
        this->CheckSubsequenceIndexes(startIndex, endIndex);
        Deque<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        int position = 0;
        while (iterator.HasValue()) {
            if (position >= startIndex && position <= endIndex) {
                result.PushBack(iterator.Get());
            }
            iterator.MoveNext();
            ++position;
        }
        return result;
    }

    template <class TResult>
    Deque<TResult> Map(std::function<TResult(T)> mapper) const {
        Deque<TResult> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            result.PushBack(mapper(iterator.Get()));
            iterator.MoveNext();
        }
        return result;
    }

    Deque<T> Where(std::function<bool(T)> predicate) const {
        Deque<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            T value = iterator.Get();
            if (predicate(value)) {
                result.PushBack(value);
            }
            iterator.MoveNext();
        }
        return result;
    }

    Deque<T> operator+(const Deque<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Deque<T>& other) const {
        return this->HasSameItems(other);
    }

    bool operator!=(const Deque<T>& other) const {
        return !(*this == other);
    }
};
