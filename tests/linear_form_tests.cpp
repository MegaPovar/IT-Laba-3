#include <gtest/gtest.h>

#include "ArraySequence.hpp"
#include "LinearForm.hpp"

// линейная форма: a0 + a1*x1 + ... + an*xn
TEST(LinearFormTests, EvaluatesAndChangesCoefficients) {
    double coefficients[] = {2.0, 3.0, -1.0, 4.0};
    LinearForm form(coefficients, 4);

    EXPECT_EQ(form.GetVariablesCount(), 3);
    EXPECT_EQ(form.GetCoefficientsCount(), 4);
    EXPECT_DOUBLE_EQ(form.GetCoefficient(0), 2.0);
    EXPECT_DOUBLE_EQ(form.GetCoefficient(2), -1.0);

    double variables[] = {10.0, 5.0, 2.0};
    MutableArraySequence<double> x(variables, 3);
    EXPECT_DOUBLE_EQ(form.Evaluate(x), 35.0);

    form.SetCoefficient(2, 1.0);
    EXPECT_DOUBLE_EQ(form.Evaluate(x), 45.0);
}

// операции с линейными формами одинаковой размерности
TEST(LinearFormTests, AddsSubtractsAndMultipliesByScalar) {
    double firstData[] = {1.0, 2.0, 3.0};
    double secondData[] = {4.0, -2.0, 1.0};
    LinearForm first(firstData, 3);
    LinearForm second(secondData, 3);

    LinearForm sum = first.Add(second);
    EXPECT_DOUBLE_EQ(sum.GetCoefficient(0), 5.0);
    EXPECT_DOUBLE_EQ(sum.GetCoefficient(1), 0.0);
    EXPECT_DOUBLE_EQ(sum.GetCoefficient(2), 4.0);

    LinearForm difference = first.Subtract(second);
    EXPECT_DOUBLE_EQ(difference.GetCoefficient(0), -3.0);
    EXPECT_DOUBLE_EQ(difference.GetCoefficient(1), 4.0);
    EXPECT_DOUBLE_EQ(difference.GetCoefficient(2), 2.0);

    LinearForm scaled = first.Multiply(2.5);
    EXPECT_DOUBLE_EQ(scaled.GetCoefficient(0), 2.5);
    EXPECT_DOUBLE_EQ(scaled.GetCoefficient(1), 5.0);
    EXPECT_DOUBLE_EQ(scaled.GetCoefficient(2), 7.5);

    LinearForm secondScaled = second.Multiply(2.0);
    EXPECT_DOUBLE_EQ(secondScaled.GetCoefficient(0), 8.0);
    EXPECT_DOUBLE_EQ(secondScaled.GetCoefficient(1), -4.0);
    EXPECT_DOUBLE_EQ(secondScaled.GetCoefficient(2), 2.0);
}

// конструктор из sequence и проверки ошибок
TEST(LinearFormTests, BuildsFromSequenceAndChecksErrors) {
    double coefficients[] = {7.0, 8.0};
    MutableArraySequence<double> sequence(coefficients, 2);
    LinearForm form(sequence);

    EXPECT_EQ(form.GetVariablesCount(), 1);
    EXPECT_DOUBLE_EQ(form.GetCoefficient(1), 8.0);

    double variables[] = {3.0};
    MutableArraySequence<double> x(variables, 1);
    EXPECT_DOUBLE_EQ(form.Evaluate(x), 31.0);

    MutableArraySequence<double> empty;
    EXPECT_THROW(LinearForm invalid(empty), std::invalid_argument);
    EXPECT_THROW(LinearForm invalidArray(coefficients, 0), std::invalid_argument);
    EXPECT_THROW(form.GetCoefficient(2), std::out_of_range);
    EXPECT_THROW(form.SetCoefficient(-1, 1.0), std::out_of_range);

    double tooManyVariables[] = {1.0, 2.0};
    MutableArraySequence<double> wrongX(tooManyVariables, 2);
    EXPECT_THROW(form.Evaluate(wrongX), std::invalid_argument);

    double otherData[] = {1.0, 2.0, 3.0};
    LinearForm other(otherData, 3);
    EXPECT_THROW(form.Add(other), std::invalid_argument);
    EXPECT_THROW(form.Subtract(other), std::invalid_argument);
}
