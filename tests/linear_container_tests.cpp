#include <gtest/gtest.h>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Stack.hpp"
#include "test_helpers.hpp"

// общий контейнер проверяю через стек
TEST(LinearContainerTests, CommonBehaviorThroughStack) {
    int source[] = {1, 2, 3, 2, 3, 4};
    Stack<int> stack(source, 6);

    EXPECT_EQ(stack.GetCount(), 6);
    EXPECT_EQ(stack.Get(0), 1);
    EXPECT_NE(dynamic_cast<const MutableListSequence<int>*>(&stack.AsSequence()), nullptr);

    int middleData[] = {2, 3};
    MutableArraySequence<int> middle(middleData, 2);
    EXPECT_TRUE(stack.ContainsSubsequence(middle));

    int missingData[] = {3, 4, 5};
    MutableArraySequence<int> missing(missingData, 3);
    EXPECT_FALSE(stack.ContainsSubsequence(missing));

    int longData[] = {1, 2, 3, 4, 5, 6, 7};
    MutableArraySequence<int> longer(longData, 7);
    EXPECT_FALSE(stack.ContainsSubsequence(longer));

    MutableArraySequence<int> empty;
    EXPECT_TRUE(stack.ContainsSubsequence(empty));

    Stack<int> emptyStack;
    EXPECT_TRUE(emptyStack.ContainsSubsequence(empty));
    EXPECT_FALSE(emptyStack.ContainsSubsequence(middle));
}

// проход по элементам через итератор
TEST(LinearContainerTests, IteratorPassesThroughItems) {
    int source[] = {4, 5, 6};
    Stack<int> stack(source, 3);

    Stack<int>::Iterator iterator = stack.Begin();
    int sum = 0;
    int count = 0;
    while (iterator.HasValue()) {
        sum += iterator.Get();
        ++count;
        iterator.MoveNext();
    }

    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 15);
    EXPECT_THROW(iterator.Get(), std::out_of_range);
}
