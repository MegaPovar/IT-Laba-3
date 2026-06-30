#pragma once

#include <stdexcept>

#include "ListSequence.hpp"

template <class T>
struct PriorityQueueItem {
    T value;
    int priority;

    PriorityQueueItem(const T& value, int priority) : value(value), priority(priority) {}
};

template <class T>
class PriorityQueue {
private:
    MutableListSequence<PriorityQueueItem<T> > data_;

    void CheckNotEmpty() const {
        if (data_.GetLength() == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
    }

public:
    PriorityQueue() = default;

    void Enqueue(const T& value, int priority) {
        PriorityQueueItem<T> item(value, priority);
        int index = 0;

        while (index < data_.GetLength() && data_.Get(index).priority >= priority) {
            ++index;
        }

        data_.InsertAt(item, index);
    }

    T Dequeue() {
        CheckNotEmpty();
        return data_.RemoveFirst().value;
    }

    T Peek() const {
        CheckNotEmpty();
        return data_.GetFirst().value;
    }

    int PeekPriority() const {
        CheckNotEmpty();
        return data_.GetFirst().priority;
    }

    int GetCount() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.GetLength() == 0;
    }

    PriorityQueueItem<T> Get(int index) const {
        return data_.Get(index);
    }

    PriorityQueue<T> Concat(const PriorityQueue<T>& other) const {
        PriorityQueue<T> result(*this);
        for (int i = 0; i < other.GetCount(); ++i) {
            PriorityQueueItem<T> item = other.Get(i);
            result.Enqueue(item.value, item.priority);
        }
        return result;
    }
};
