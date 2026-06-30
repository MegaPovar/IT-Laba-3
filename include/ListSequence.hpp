#pragma once

#include <stdexcept>
#include "LinkedList.hpp"
#include "Sequence.hpp"

template <class T>
class ListSequenceBase : public Sequence<T> {
protected:
    LinkedList<T> items;

    virtual ListSequenceBase<T>* Instance() = 0;
    virtual ListSequenceBase<T>* NewEmpty() const = 0;

    ListSequenceBase<T>* AppendInternal(const T& item) {
        items.Append(item);
        return this;
    }

    ListSequenceBase<T>* PrependInternal(const T& item) {
        items.Prepend(item);
        return this;
    }

    ListSequenceBase<T>* InsertInternal(const T& item, int index) {
        items.InsertAt(item, index);
        return this;
    }

public:
    ListSequenceBase() : items() {}
    ListSequenceBase(T* data, int count) : items(data, count) {}
    explicit ListSequenceBase(const LinkedList<T>& list) : items(list) {}

    const T& GetFirst() const override {
        return items.GetFirst();
    }

    const T& GetLast() const override {
        return items.GetLast();
    }

    const T& Get(int index) const override {
        return items.Get(index);
    }

    int GetLength() const override {
        return items.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetLength() || endIndex >= GetLength()) {
            throw std::out_of_range("Subsequence indices are out of range");    
        }
        if (startIndex > endIndex) {
            throw std::invalid_argument("startIndex cannot be greater than endIndex");
        }

        ListSequenceBase<T>* result = NewEmpty();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(items.Get(i));
        }
        return result;
    }

    Sequence<T>* Append(const T& item) override {
        return Instance()->AppendInternal(item);
    }

    Sequence<T>* Prepend(const T& item) override {
        return Instance()->PrependInternal(item);
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        return Instance()->InsertInternal(item, index);
    }

    Sequence<T>* Concat(const Sequence<T>& list) override {
        ListSequenceBase<T>* result = Instance();
        Sequence<T>* copy = nullptr;
        const Sequence<T>* source = &list;
        try {
            if (&list == this) {
                copy = list.Clone();
                source = copy;
            }
            for (int i = 0; i < source->GetLength(); ++i) {
                result->AppendInternal(source->Get(i));
            }
            delete copy;
            return result;
        } catch (...) {
            delete copy;
            if (result != this) {
                delete result;
            }
            throw;
        }
    }

    T& operator[](int index) override {
        return items[index];
    }

    const T& operator[](int index) const override {
        return items[index];
    }
};

template <class T>
class MutableListSequence : public ListSequenceBase<T> {
protected:
    using ListSequenceBase<T>::items;

    ListSequenceBase<T>* Instance() override {
        return this;
    }

    ListSequenceBase<T>* NewEmpty() const override {
        return new MutableListSequence<T>();
    }

public:
    MutableListSequence() : ListSequenceBase<T>() {}
    MutableListSequence(T* data, int count) : ListSequenceBase<T>(data, count) {}
    explicit MutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequenceBase<T>(other.items) {}

    T RemoveFirst() {
        return items.RemoveFirst();
    }

    T RemoveLast() {
        return items.RemoveLast();
    }

    void InsertBefore(const T& item, bool (*shouldInsertBefore)(const T&, const T&)) {
        if (shouldInsertBefore == nullptr) {
            throw std::invalid_argument("Insert condition is empty");
        }

        MutableListSequence<T> result;
        bool inserted = false;
        auto iterator = items.Begin();
        while (iterator.HasValue()) {
            const T& current = iterator.Get();
            if (!inserted && shouldInsertBefore(item, current)) {
                result.Append(item);
                inserted = true;
            }
            result.Append(current);
            iterator.MoveNext();
        }
        if (!inserted) {
            result.Append(item);
        }
        items = result.items;
    }

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

template <class T>
class ImmutableListSequence : public ListSequenceBase<T> {
protected:
    ListSequenceBase<T>* Instance() override {
        return new ImmutableListSequence<T>(*this);
    }

    ListSequenceBase<T>* NewEmpty() const override {
        return new ImmutableListSequence<T>();
    }

public:
    ImmutableListSequence() : ListSequenceBase<T>() {}
    ImmutableListSequence(T* data, int count) : ListSequenceBase<T>(data, count) {}
    explicit ImmutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequenceBase<T>(other.items) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

#include "SequenceAlgorithms.hpp"
