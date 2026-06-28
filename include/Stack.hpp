#pragma once

#include <functional> // function для алгоритмов
#include <stdexcept>

#include "LinearContainer.hpp"

template <class T>
class Stack : public LinearContainer<T> {
public:
    Stack() : LinearContainer<T>() {} // пустой стек
    explicit Stack(const Sequence<T>& sequence) : LinearContainer<T>(sequence) {} // конструктор из sequence для создания стека из любой последовательности
    Stack(T* data, int count) : LinearContainer<T>(data, count) {}

    T Peek() const {
        if (this->IsEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return this->items->GetLast();
    }

    void Push(const T& value) { 
        this->AppendBack(value);
    }

    T Pop() {
        if (this->IsEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return this->RemoveBack();
    }

    Stack<T> Concat(const Stack<T>& other) const { 
        Stack<T> result(*this);
        typename LinearContainer<T>::Iterator iterator = other.Begin();
        while (iterator.HasValue()) {
            result.Push(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    Stack<T> GetSubstack(int startIndex, int endIndex) const { // кусок стека по индексам
        this->CheckSubsequenceIndexes(startIndex, endIndex);
        Stack<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        int position = 0;
        while (iterator.HasValue()) {
            if (position >= startIndex && position <= endIndex) {
                result.Push(iterator.Get());
            }
            iterator.MoveNext();
            ++position;
        }
        return result;
    }

    template <class TResult>
    Stack<TResult> Map(std::function<TResult(T)> mapper) const { // применить функцию ко всем элементам
        Stack<TResult> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            result.Push(mapper(iterator.Get()));
            iterator.MoveNext();
        }
        return result;
    }

    Stack<T> Where(std::function<bool(T)> predicate) const { // оставить элементы по условию
        Stack<T> result;
        typename LinearContainer<T>::Iterator iterator = this->Begin();
        while (iterator.HasValue()) {
            T value = iterator.Get();
            if (predicate(value)) {
                result.Push(value);
            }
            iterator.MoveNext();
        }
        return result;
    }

    Stack<T> operator+(const Stack<T>& other) const { //c = a + b вызывая concat
        return Concat(other);
    }

    bool operator==(const Stack<T>& other) const { // сравнение поэлементно
        return this->HasSameItems(other);
    }

    bool operator!=(const Stack<T>& other) const { // обратное к ==
        return !(*this == other);
    }

};
