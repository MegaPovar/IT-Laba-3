#pragma once

#include <cstddef>
#include <functional>
#include <limits>

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "ICollection.hpp"
#include "SequenceAlgorithms.hpp"

template <class T>
class Stack : public ICollection<T> {
private:
    template <class>
    friend class Stack;

    Sequence<T>* items;

    explicit Stack(Sequence<T>* sequence, bool takeOwnership) : items(nullptr) {
        if (sequence == nullptr) {
            throw InvalidArgument("Stack sequence cannot be null");
        }
        items = takeOwnership ? sequence : sequence->Clone();
    }

    void ReplaceItems(Sequence<T>* next) {
        if (next != items) {
            delete items;
            items = next;
        }
    }

    static int ToIntIndex(std::size_t index) {
        if (index > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
            throw IndexOutOfRange("Stack index is too large");
        }
        return static_cast<int>(index);
    }

public:
    Stack() : items(new MutableArraySequence<T>()) {}

    explicit Stack(const Sequence<T>& sequence) : items(sequence.Clone()) {}

    Stack(T* data, int count) : items(new MutableArraySequence<T>(data, count)) {}

    Stack(const Stack<T>& other) : items(other.items->Clone()) {}

    Stack<T>& operator=(const Stack<T>& other) {
        if (this == &other) {
            return *this;
        }
        Sequence<T>* copy = other.items->Clone();
        delete items;
        items = copy;
        return *this;
    }

    ~Stack() {
        delete items;
    }

    int GetSize() const {
        return items->GetLength();
    }

    std::size_t GetCount() const override {
        return static_cast<std::size_t>(items->GetLength());
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

    T Get(std::size_t index) const override {
        return items->Get(ToIntIndex(index));
    }

    T Peek() const {
        if (IsEmpty()) {
            throw IndexOutOfRange("Stack is empty");
        }
        return items->GetLast();
    }

    void Push(const T& value) {
        ReplaceItems(items->Append(value));
    }

    T Pop() {
        if (IsEmpty()) {
            throw IndexOutOfRange("Stack is empty");
        }
        T value = items->GetLast();
        ReplaceItems(items->Slice(GetSize() - 1, 1));
        return value;
    }

    Stack<T> Concat(const Stack<T>& other) const {
        Sequence<T>* copy = items->Clone();
        Sequence<T>* result = copy->Concat(other.items);
        if (result != copy) {
            delete copy;
        }
        return Stack<T>(result, true);
    }

    Stack<T> GetSubstack(int startIndex, int endIndex) const {
        Sequence<T>* result = items->GetSubsequence(startIndex, endIndex);
        return Stack<T>(result, true);
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
    Stack<TResult> Map(std::function<TResult(T)> mapper) const {
        Sequence<TResult>* result = items->template Map<TResult>(mapper);
        return Stack<TResult>(result, true);
    }

    Stack<T> Where(std::function<bool(T)> predicate) const {
        Sequence<T>* result = items->Where(predicate);
        return Stack<T>(result, true);
    }

    template <class TResult>
    TResult Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const {
        return items->template Reduce<TResult>(reducer, start);
    }

    Stack<T> operator+(const Stack<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Stack<T>& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }
        for (int i = 0; i < GetSize(); ++i) {
            if (!(items->Get(i) == other.items->Get(i))) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }

    const Sequence<T>& AsSequence() const {
        return *items;
    }
};
