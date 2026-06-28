#pragma once

#include "ArraySequence.hpp"
#include "SequenceIterator.hpp"
#include <stdexcept>

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::Map(std::function<TResult(T)> mapper) const { // применяем mapper к каждому элементу
    const TSelf& sequence = static_cast<const TSelf&>(*this); // CRTP: берем настоящий тип
    Sequence<TResult>* result = new MutableArraySequence<TResult>(); // результат может быть другого типа
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        result->Append(mapper(iterator.Get()));
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::MapIndexed(std::function<TResult(T, int)> mapper) const { // map + индекс 
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
Sequence<T>* SequenceOperations<T, TSelf>::Where(std::function<bool(T)> predicate) const { // фильтр по условию (потом %2 )
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
TResult SequenceOperations<T, TSelf>::Reduce(std::function<TResult(TResult, T)> reducer, TResult start) const { // свертка в одно значение
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    TResult result = start; // начальное значение
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        result = reducer(result, iterator.Get());
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
template <class TResult>
Sequence<TResult>* SequenceOperations<T, TSelf>::FlatMap(std::function<Sequence<TResult>*(T)> mapper) const { // каждый элемент дает последовательность
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<TResult>* result = new MutableArraySequence<TResult>(); 
    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        Sequence<TResult>* part = mapper(iterator.Get()); // временная часть результата
        SequenceIterator<TResult> partIterator(*part);
        while (partIterator.HasValue()) {
            result->Append(partIterator.Get());
            partIterator.MoveNext();
        }
        delete part; // удаляем временную часть, так как она уже склеена в результат
        iterator.MoveNext();
    }
    return result;
}

template <class T, class TSelf>
Sequence<Sequence<T>*>* SequenceOperations<T, TSelf>::Split(std::function<bool(T)> separator) const { // разбить на куски
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    Sequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>(); // список кусков
    Sequence<T>* current = new MutableArraySequence<T>(); // текущий кусок

    SequenceIterator<T> iterator(sequence);
    while (iterator.HasValue()) {
        T value = iterator.Get();
        if (separator(value)) {
            result->Append(current);
            current = new MutableArraySequence<T>(); // начинаем новый кусок
        } else {
            current->Append(value);
        }
        iterator.MoveNext();
    }
    result->Append(current);
    return result;
}

template <class T, class TSelf>
Sequence<T>* SequenceOperations<T, TSelf>::Slice(int index, int count, const Sequence<T>* inserted) const { // удалить count элементов и вставить другие
    const TSelf& sequence = static_cast<const TSelf&>(*this);
    if (count < 0) {
        throw std::invalid_argument("Slice count cannot be negative");
    }

    int length = sequence.GetLength(); // длина исходной последовательности
    int start;
    if (index < 0) {
        start = length + index; // отрицательный индекс считаем с конца
    } else {
        start = index;
    }
    if (start < 0 || start > length) {
        throw std::out_of_range("Slice index is out of range");
    }
    if (start + count > length) {
        throw std::out_of_range("Slice count is out of range");
    }

    Sequence<T>* result = new MutableArraySequence<T>(); // новая последовательность результата
    SequenceIterator<T> iterator(sequence);
    int position = 0;
    while (iterator.HasValue()) {
        if (position == start && inserted != nullptr) { // если есть что вставить
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
Sequence<std::pair<T, T> >* Zip(const Sequence<T>* first, const Sequence<T>* second) { // соединить две последовательности парами
    int length;
    if (first->GetLength() < second->GetLength()) {
        length = first->GetLength();
    } else {
        length = second->GetLength();
    }
    Sequence<std::pair<T, T> >* result = new MutableArraySequence<std::pair<T, T> >(); // pair = пара значений
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
