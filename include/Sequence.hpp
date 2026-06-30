#pragma once

#include <functional>
#include <utility>

template <class T>
class Sequence;

template <class T, class TSelf>
class SequenceOperations {
public:
    template <class TResult>
    Sequence<TResult>* Map(std::function<TResult(T)> mapper) const;

    template <class TResult>
    Sequence<TResult>* MapIndexed(std::function<TResult(T, int)> mapper) const;

    Sequence<T>* Where(std::function<bool(T)> predicate) const;

    template <class TResult>
    TResult Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const;

    Sequence<T>* Slice(int index, int count, const Sequence<T>* inserted = nullptr) const;
};

template <class T>
class Sequence : public SequenceOperations<T, Sequence<T>> {
public:
    virtual ~Sequence() {}

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0; 
    virtual const T& Get(int index) const = 0; 
    virtual int GetLength() const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* Append(const T& item) = 0; 
    virtual Sequence<T>* Prepend(const T& item) = 0; 
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0; 
    virtual Sequence<T>* Concat(const Sequence<T>& list) = 0;
    virtual Sequence<T>* Clone() const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

};

template <class T>
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second);
