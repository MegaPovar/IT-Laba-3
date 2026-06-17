#pragma once

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "ICollection.hpp"
#include "SequenceAlgorithms.hpp"

template <class T>
class LinearContainer : public ICollection<T> {
protected:
    Sequence<T>* items; // общий контейнер хранит данные как sequence

    explicit LinearContainer(Sequence<T>* sequence, bool takeOwnership) : items(nullptr) {
        if (sequence == nullptr) {
            throw InvalidArgument("LinearContainer sequence cannot be null");
        }
        items = takeOwnership ? sequence : sequence->Clone();
    }

    void ReplaceItems(Sequence<T>* next) {
        if (next != items) {
            delete items;
            items = next;
        }
    }

    void AppendBack(const T& value) {
        ReplaceItems(items->Append(value));
    }

    void PrependFront(const T& value) {
        ReplaceItems(items->Prepend(value));
    }

    T RemoveBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange("Container is empty");
        }
        T value = items->GetLast();
        ReplaceItems(items->Slice(GetCount() - 1, 1));
        return value;
    }

    T RemoveFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange("Container is empty");
        }
        T value = items->GetFirst();
        ReplaceItems(items->Slice(0, 1));
        return value;
    }

    Sequence<T>* ConcatToSequence(const LinearContainer<T>& other) const {
        Sequence<T>* copy = items->Clone();
        Sequence<T>* result = copy->Concat(other.items);
        if (result != copy) {
            delete copy;
        }
        return result;
    }

public:
    LinearContainer() : items(new MutableArraySequence<T>()) {}

    explicit LinearContainer(const Sequence<T>& sequence) : items(sequence.Clone()) {}

    LinearContainer(T* data, int count) : items(new MutableArraySequence<T>(data, count)) {}

    LinearContainer(const LinearContainer<T>& other) : items(other.items->Clone()) {}

    LinearContainer<T>& operator=(const LinearContainer<T>& other) {
        if (this == &other) {
            return *this;
        }
        Sequence<T>* copy = other.items->Clone();
        delete items;
        items = copy;
        return *this;
    }

    virtual ~LinearContainer() {
        delete items;
    }

    int GetCount() const override {
        return items->GetLength();
    }

    int GetSize() const {
        return GetCount();
    }

    bool IsEmpty() const {
        return GetCount() == 0;
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    bool ContainsSubsequence(const Sequence<T>& subsequence) const {
        int sourceLength = items->GetLength();
        int subLength = subsequence.GetLength();
        if (subLength == 0) {
            return true;
        }
        if (subLength > sourceLength) {
            return false;
        }
        for (int start = 0; start <= sourceLength - subLength; ++start) {
            bool equal = true;
            for (int offset = 0; offset < subLength; ++offset) {
                if (!(items->Get(start + offset) == subsequence.Get(offset))) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                return true;
            }
        }
        return false;
    }

    template <class TResult>
    TResult Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const {
        return items->template Reduce<TResult>(reducer, start);
    }

    const Sequence<T>& AsSequence() const {
        return *items;
    }
};
