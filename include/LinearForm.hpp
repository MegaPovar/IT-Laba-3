#pragma once

#include <stdexcept>

#include "ListSequence.hpp"
#include "SequenceIterator.hpp"

template <class T>
class LinearForm {
private:
    MutableListSequence<T> coefficients;

    void CheckCoefficientIndex(int index) const {
        if (index < 0 || index >= coefficients.GetLength()) {
            throw std::out_of_range("LinearForm coefficient index is out of range");
        }
    }

    void CheckSameVariablesCount(const LinearForm<T>& other) const {
        if (GetVariablesCount() != other.GetVariablesCount()) {
            throw std::invalid_argument("Linear forms have different variables count");
        }
    }

public:
    explicit LinearForm(const Sequence<T>& source) {
        if (source.GetLength() == 0) {
            throw std::invalid_argument("LinearForm needs at least one coefficient");
        }

        SequenceIterator<T> iterator(source);
        while (iterator.HasValue()) {
            coefficients.Append(iterator.Get());
            iterator.MoveNext();
        }
    }

    LinearForm(T* data, int count) {
        if (count <= 0) {
            throw std::invalid_argument("LinearForm needs at least one coefficient");
        }
        if (data == nullptr) {
            throw std::invalid_argument("LinearForm source cannot be null");
        }

        for (int i = 0; i < count; ++i) {
            coefficients.Append(data[i]);
        }
    }

    int GetVariablesCount() const {
        return coefficients.GetLength() - 1;
    }

    int GetCoefficientsCount() const {
        return coefficients.GetLength();
    }

    const T& GetCoefficient(int index) const {
        CheckCoefficientIndex(index);
        return coefficients.Get(index);
    }

    void SetCoefficient(int index, const T& value) {
        CheckCoefficientIndex(index);
        coefficients[index] = value;
    }

    T Evaluate(const Sequence<T>& variables) const {
        if (variables.GetLength() != GetVariablesCount()) {
            throw std::invalid_argument("Variables count does not match linear form");
        }

        T result = coefficients.Get(0);
        SequenceIterator<T> iterator(variables);
        int index = 1;
        while (iterator.HasValue()) {
            result = result + coefficients.Get(index) * iterator.Get();
            iterator.MoveNext();
            ++index;
        }
        return result;
    }

    LinearForm<T> Add(const LinearForm<T>& other) const {
        CheckSameVariablesCount(other);
        MutableListSequence<T> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) + other.coefficients.Get(i));
        }
        return LinearForm<T>(resultCoefficients);
    }

    LinearForm<T> Subtract(const LinearForm<T>& other) const {
        CheckSameVariablesCount(other);
        MutableListSequence<T> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) - other.coefficients.Get(i));
        }
        return LinearForm<T>(resultCoefficients);
    }

    LinearForm<T> Multiply(const T& scalar) const {
        MutableListSequence<T> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) * scalar);
        }
        return LinearForm<T>(resultCoefficients);
    }
};
