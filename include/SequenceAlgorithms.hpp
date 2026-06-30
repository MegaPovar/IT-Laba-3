#pragma once

#include "ArraySequence.hpp"
#include "SequenceIterator.hpp"
#include <stdexcept>

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::Map(std::function<TResult(T)> mapper) const {
    if (!mapper) {
        throw std::invalid_argument("Map function is empty");
    }

    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        result->Append(mapper(iterator.Get()));
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::MapIndexed(std::function<TResult(T, int)> mapper) const {
    if (!mapper) {
        throw std::invalid_argument("MapIndexed function is empty");
    }

    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<TResult>* result = new MutableArraySequence<TResult>();
    SequenceIterator<T> iterator(sequence);
    int index = 0;
    while (iterator.HasValue()) {
        result->Append(mapper(iterator.Get(), index));
        iterator.MoveNext();
        ++index;
    }
    return result;
}

template <class T, class TSelf>
Sequence<T>* SequenceOperations<T, TSelf>::Where(std::function<bool(T)> predicate) const {
    if (!predicate) {
        throw std::invalid_argument("Where predicate is empty");
    }

    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<T>* result = new MutableArraySequence<T>();
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        T value = iterator.Get();
        if (predicate(value)) {
            result->Append(value);
        }
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
TResult SequenceOperations<T, TSelf>::Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const {
    if (!reducer) {
        throw std::invalid_argument("Reduce function is empty");
    }

    const TSelf& sequence = static_cast<const TSelf&>(*this);
    TResult result = start;
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        result = reducer(result, iterator.Get());
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
Sequence<T>* SequenceOperations<T, TSelf>::Slice(int index, int count, const Sequence<T>* inserted) const {
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    if (count < 0) {
        throw std::invalid_argument("Slice count cannot be negative");
    }

    int length = sequence.GetLength();
    int start;
    if (index < 0) {
        start = length + index;
    } else {
        start = index;
    }
    if (start < 0 || start > length) {
        throw std::out_of_range("Slice index is out of range");
    }
    if (start + count > length) {
        throw std::out_of_range("Slice count is out of range");
    }

    Sequence<T>* result = new MutableArraySequence<T>();
    SequenceIterator<T> iterator(sequence);
    int position = 0;
    while (iterator.HasValue()) {
        if (position == start && inserted != nullptr) {
            SequenceIterator<T> insertedIterator(*inserted);
            while (insertedIterator.HasValue()) {
                result->Append(insertedIterator.Get());
                insertedIterator.MoveNext();
            }
        }
        if (position < start || position >= start + count) {
            result->Append(iterator.Get());
        }
        iterator.MoveNext();
        ++position;
    }
    if (start == length && inserted != nullptr) {
        SequenceIterator<T> insertedIterator(*inserted);
        while (insertedIterator.HasValue()) {
            result->Append(insertedIterator.Get());
            insertedIterator.MoveNext();
        }
    }
    return result;
}

template <class T>
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second) {
    if (first == nullptr || second == nullptr) {
        throw std::invalid_argument("Zip sequences cannot be null");
    }

    int length;
    if (first->GetLength() < second->GetLength()) {
        length = first->GetLength();
    } else {
        length = second->GetLength();
    }
    Sequence<std::pair<T, T> >* result = new MutableArraySequence<std::pair<T, T> >();
    SequenceIterator<T> firstIterator(*first);
    SequenceIterator<T> secondIterator(*second);
    int count = 0;
    while (count < length) {
        result->Append(std::make_pair(firstIterator.Get(), secondIterator.Get()));
        firstIterator.MoveNext();
        secondIterator.MoveNext();
        ++count;
    }
    return result;
}
