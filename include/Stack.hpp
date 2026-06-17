#pragma once

#include <functional> // function для алгоритмов

#include "LinearContainer.hpp"

template <class T>
class Stack : public LinearContainer<T> {
private:
    template <class>
    friend class Stack; // для обращения шаблонных классов друг к другу

    explicit Stack(Sequence<T>* sequence, bool takeOwnership) : LinearContainer<T>(sequence, takeOwnership) {} // внутренний конструктор. оборачиваем sequence в стек

public:
    Stack() : LinearContainer<T>() {} // пустой стек
    explicit Stack(const Sequence<T>& sequence) : LinearContainer<T>(sequence) {} // конструктор из sequence для создания стека из любой последовательности
    Stack(T* data, int count) : LinearContainer<T>(data, count) {}

    T Peek() const {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Stack is empty");
        }
        return this->items->GetLast();
    }

    void Push(const T& value) { 
        this->AppendBack(value);
    }

    T Pop() {
        if (this->IsEmpty()) {
            throw IndexOutOfRange("Stack is empty");
        }
        return this->RemoveBack();
    }

    Stack<T> Concat(const Stack<T>& other) const { 
        Sequence<T>* result = this->ConcatToSequence(other);
        return Stack<T>(result, true);
    }

    Stack<T> GetSubstack(int startIndex, int endIndex) const { // кусок стека по индексам
        Sequence<T>* result = this->items->GetSubsequence(startIndex, endIndex);
        return Stack<T>(result, true);
    }

    template <class TResult>
    Stack<TResult> Map(std::function<TResult(T)> mapper) const { // применить функцию ко всем элементам
        Sequence<TResult>* result = this->items->template Map<TResult>(mapper);
        return Stack<TResult>(result, true);
    }

    Stack<T> Where(std::function<bool(T)> predicate) const { // оставить элементы по условию
        Sequence<T>* result = this->items->Where(predicate);
        return Stack<T>(result, true);
    }

    Stack<T> operator+(const Stack<T>& other) const { //c = a + b вызывая concat
        return Concat(other);
    }

    bool operator==(const Stack<T>& other) const { // сравнение поэлементно
        if (this->GetSize() != other.GetSize()) {
            return false;
        }
        for (int i = 0; i < this->GetSize(); ++i) {
            if (!(this->items->Get(i) == other.items->Get(i))) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Stack<T>& other) const { // обратное к ==
        return !(*this == other);
    }

};
