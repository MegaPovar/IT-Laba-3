#pragma once

#include <stdexcept>
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"
#include "SequenceIterator.hpp"

template <class T> //произвольный тип данных 
class LinearContainer {
protected:
    MutableListSequence<T>* items;

    void ReplaceItems(MutableListSequence<T>* next) {
        if (next != items) {
            delete items;
            items = next;
        }
    }

    MutableListSequence<T>* CopyToListSequence(const Sequence<T>& sequence) const {
        MutableListSequence<T>* result = new MutableListSequence<T>();
        SequenceIterator<T> iterator(sequence);
        while (iterator.HasValue()) {
            result->Append(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    void AppendBack(const T& value) { // const для переменной и временных значений
        items->Append(value);
    }

    void PrependFront(const T& value) {
        items->Prepend(value);
    }

    T RemoveBack() { // [10, 20, 30] = [10, 20], возвращаем 30
        if (IsEmpty()) {
            throw std::out_of_range("Container is empty");
        }
        T value = items->GetLast();
        MutableListSequence<T>* next = new MutableListSequence<T>();
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
            throw std::out_of_range("Container is empty");
        }
        T value = items->GetFirst();
        MutableListSequence<T>* next = new MutableListSequence<T>();
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
            throw std::out_of_range("Container subsequence index is out of range");
        }
        if (startIndex > endIndex) {
            throw std::invalid_argument("startIndex cannot be greater than endIndex");
        }
    }

public: //конструкторы - ? 
    typedef SequenceIterator<T> Iterator;

    LinearContainer() : items(new MutableListSequence<T>()) {}

    explicit LinearContainer(const Sequence<T>& sequence) : items(nullptr) { //почему есть/нету explicit
        items = CopyToListSequence(sequence);
    }

    LinearContainer(T* data, int count) : items(new MutableListSequence<T>(data, count)) {}

    LinearContainer(const LinearContainer<T>& other) : items(nullptr) {
        items = CopyToListSequence(*other.items);
    }

    LinearContainer<T>& operator=(const LinearContainer<T>& other) { // перегрузка оператора присваивания - ?
        if (this == &other) {
            return *this;
        }
        MutableListSequence<T>* copy = CopyToListSequence(*other.items); 
        delete items;
        items = copy;
        return *this;
    }

    virtual ~LinearContainer() { //виртуальный деструктор, чтобы наследники нормально удалялись через LinearContainer*
        delete items;
    }
    
    int GetCount() const {
        return items->GetLength();
    }

    bool IsEmpty() const {
        return GetCount() == 0;
    }

    T Get(int index) const { //произвольный тип данных сука не абстрактный 
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
            Iterator source = Begin();
            for (int skip = 0; skip < start; ++skip) {
                source.MoveNext();
            }
            Iterator sub(subsequence);
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
