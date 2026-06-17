#pragma once

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "SequenceAlgorithms.hpp"
#include "SequenceIterator.hpp"

template <class T>
class LinearContainer {
protected:
    Sequence<T>* items;

    void ReplaceItems(Sequence<T>* next) {
        if (next != items) {
            delete items;
            items = next;
        }
    }

    void AppendBack(const T& value) { // const для переменной и временныхъ значений
        ReplaceItems(items->Append(value));
    }

    void PrependFront(const T& value) {
        ReplaceItems(items->Prepend(value));
    }

    T RemoveBack() { // [10, 20, 30] = [10, 20], возвращаем 30
        if (IsEmpty()) {
            throw IndexOutOfRange("Container is empty");
        }
        T value = items->GetLast();
        Sequence<T>* next = new MutableArraySequence<T>();
        SequenceIterator<T> iterator = Begin();
        while (iterator.HasValue()) {
            T current = iterator.Get();
            iterator.MoveNext();
            if (iterator.HasValue()) {
                next->Append(current);
            }
        }
        ReplaceItems(next);
        return value;
    }

    T RemoveFront() { // [10, 20, 30] = [20, 30], возвращаем 10
        if (IsEmpty()) {
            throw IndexOutOfRange("Container is empty");
        }
        T value = items->GetFirst();
        Sequence<T>* next = new MutableArraySequence<T>();
        SequenceIterator<T> iterator = Begin();
        if (iterator.HasValue()) {
            iterator.MoveNext();
        }
        while (iterator.HasValue()) {
            next->Append(iterator.Get());
            iterator.MoveNext();
        }
        ReplaceItems(next);
        return value;
    }

    bool HasSameItems(const LinearContainer<T>& other) const {
        if (GetCount() != other.GetCount()) {
            return false;
        }
        SequenceIterator<T> first = Begin();
        SequenceIterator<T> second = other.Begin();
        while (first.HasValue() && second.HasValue()) {
            if (!(first.Get() == second.Get())) {
                return false;
            }
            first.MoveNext();
            second.MoveNext();
        }
        return true;
    }

    void CheckSubsequenceIndexes(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetCount() || endIndex >= GetCount()) {
            throw IndexOutOfRange("Container subsequence index is out of range");
        }
        if (startIndex > endIndex) {
            throw InvalidArgument("startIndex cannot be greater than endIndex");
        }
    }

public:
    typedef SequenceIterator<T> Iterator;

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

    int GetCount() const {
        return items->GetLength();
    }

    bool IsEmpty() const {
        return GetCount() == 0;
    }

    T Get(int index) const {
        return items->Get(index);
    }

    Iterator Begin() const {
        return Iterator(*items);
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
            SequenceIterator<T> source = Begin();
            for (int skip = 0; skip < start; ++skip) {
                source.MoveNext();
            }
            SequenceIterator<T> sub(subsequence);
            bool equal = true;
            while (sub.HasValue()) {
                if (!(source.Get() == sub.Get())) {
                    equal = false;
                    break;
                }
                source.MoveNext();
                sub.MoveNext();
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
