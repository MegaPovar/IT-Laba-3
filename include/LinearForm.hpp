#pragma once

#include <stdexcept>

#include "ListSequence.hpp"
#include "SequenceIterator.hpp"

class LinearForm {
private:
    MutableListSequence<double> coefficients;

    void CheckCoefficientIndex(int index) const {
        if (index < 0 || index >= coefficients.GetLength()) {
            throw std::out_of_range("LinearForm coefficient index is out of range");
        }
    }

    void CheckSameVariablesCount(const LinearForm& other) const {
        if (GetVariablesCount() != other.GetVariablesCount()) {
            throw std::invalid_argument("Linear forms have different variables count");
        }
    }

public:
    explicit LinearForm(const Sequence<double>& source) {
        if (source.GetLength() == 0) {
            throw std::invalid_argument("LinearForm needs at least one coefficient");
        }

        SequenceIterator<double> iterator(source);
        while (iterator.HasValue()) {
            coefficients.Append(iterator.Get());
            iterator.MoveNext();
        }
    }

    LinearForm(double* data, int count) {
        if (count <= 0) {
            throw std::invalid_argument("LinearForm needs at least one coefficient");
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

    double GetCoefficient(int index) const {
        CheckCoefficientIndex(index);
        return coefficients.Get(index);
    }

    void SetCoefficient(int index, double value) {
        CheckCoefficientIndex(index);
        coefficients[index] = value;
    }

    double Evaluate(const Sequence<double>& variables) const {
        if (variables.GetLength() != GetVariablesCount()) {
            throw std::invalid_argument("Variables count does not match linear form");
        }

        double result = coefficients.Get(0);
        SequenceIterator<double> iterator(variables);
        int index = 1;
        while (iterator.HasValue()) {
            result += coefficients.Get(index) * iterator.Get();
            iterator.MoveNext();
            ++index;
        }
        return result;
    }

    LinearForm Add(const LinearForm& other) const {
        CheckSameVariablesCount(other);
        MutableListSequence<double> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) + other.coefficients.Get(i));
        }
        return LinearForm(resultCoefficients);
    }

    LinearForm Subtract(const LinearForm& other) const {
        CheckSameVariablesCount(other);
        MutableListSequence<double> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) - other.coefficients.Get(i));
        }
        return LinearForm(resultCoefficients);
    }

    LinearForm Multiply(double scalar) const {
        MutableListSequence<double> resultCoefficients;
        for (int i = 0; i < coefficients.GetLength(); ++i) {
            resultCoefficients.Append(coefficients.Get(i) * scalar);
        }
        return LinearForm(resultCoefficients);
    }
};
