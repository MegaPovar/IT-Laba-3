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

    static bool HasHigherPriority(const PriorityQueueItem<T>& item, const PriorityQueueItem<T>& current) {
        return item.priority > current.priority;
    }

    void CheckNotEmpty() const {
        if (data_.GetLength() == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
    }

public:
    PriorityQueue() = default;

    void Enqueue(const T& value, int priority) {
        PriorityQueueItem<T> item(value, priority);
        data_.InsertBefore(item, HasHigherPriority);
    }

    T Dequeue() {
        CheckNotEmpty();
        return data_.RemoveFirst().value;
    }

    const T& Peek() const {
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

    PriorityQueue<T> Concat(const PriorityQueue<T>& other) const {
        PriorityQueue<T> result(*this);
        PriorityQueue<T> source(other);
        while (!source.IsEmpty()) {
            int priority = source.PeekPriority();
            T value = source.Dequeue();
            result.Enqueue(value, priority);
        }
        return result;
    }
};
