#include <gtest/gtest.h>

#include <string>

#include "ArraySequence.hpp"
#include "Deque.hpp"
#include "ListSequence.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "test_helpers.hpp"

// основные штуки стека
TEST(StackTests, PushPopMapWhereReduceAndOperators) {
    Stack<int> stack;
    EXPECT_TRUE(stack.IsEmpty());
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    EXPECT_EQ(stack.GetCount(), 3);
    EXPECT_EQ(stack.Get(1), 20);
    EXPECT_EQ(stack.Peek(), 30);
    EXPECT_EQ(stack.Pop(), 30);
    EXPECT_EQ(stack.Peek(), 20);

    Stack<int> copy(stack); // проверка копирования и сравнения
    EXPECT_EQ(copy, stack);
    copy.Push(40);
    EXPECT_NE(copy, stack);

    Stack<int> mapped = stack.Map<int>(DoubleTestInt); // map для стека
    EXPECT_EQ(mapped.Get(0), 20);
    EXPECT_EQ(mapped.Get(1), 40);

    Stack<int> filtered = copy.Where(AtLeastTwenty); // фильтрация
    EXPECT_EQ(filtered.GetCount(), 2);
    EXPECT_EQ(filtered.Get(0), 20);
    EXPECT_EQ(filtered.Get(1), 40);

    int sum = copy.Reduce<int>(SumTestInt, 0); // сумма через reduce
    EXPECT_EQ(sum, 70);

    Stack<int> concatenated = stack + filtered;
    EXPECT_EQ(concatenated.GetCount(), 4);
    EXPECT_EQ(concatenated.Get(0), 10);
    EXPECT_EQ(concatenated.Get(3), 40);

    Stack<int> sub = concatenated.GetSubstack(1, 2);
    EXPECT_EQ(sub.GetCount(), 2);
    EXPECT_EQ(sub.Get(0), 20);
    EXPECT_EQ(sub.Get(1), 20);
    EXPECT_TRUE(concatenated.ContainsSubsequence(sub.AsSequence()));

    Stack<int> empty;
    EXPECT_THROW(empty.Pop(), std::out_of_range);
    EXPECT_THROW(empty.Peek(), std::out_of_range);
    EXPECT_THROW(empty.Get(0), std::out_of_range);
    EXPECT_THROW(stack.GetSubstack(-1, 1), std::out_of_range);
    EXPECT_THROW(stack.GetSubstack(0, 10), std::out_of_range);
    EXPECT_THROW(stack.GetSubstack(1, 0), std::invalid_argument);
}

// стек не только на массиве и не только int
TEST(StackTests, WorksWithDifferentStorageAndStringType) {
    int source[] = {1, 2, 3};
    MutableListSequence<int> listSequence(source, 3);
    Stack<int> listStack(listSequence);
    listStack.Push(4);
    EXPECT_EQ(listStack.Pop(), 4);
    EXPECT_EQ(listStack.Peek(), 3);

    ImmutableArraySequence<int> immutableSequence(source, 3);
    Stack<int> immutableStack(immutableSequence);
    immutableStack.Push(4);
    EXPECT_EQ(immutableSequence.GetLength(), 3);
    EXPECT_EQ(immutableStack.GetCount(), 4);

    Stack<int> assigned;
    assigned.Push(100);
    assigned = listStack;
    EXPECT_EQ(assigned, listStack);
    assigned.Push(10);
    EXPECT_NE(assigned, listStack);
    Stack<int>* sameStack = &assigned;
    assigned = *sameStack;
    EXPECT_EQ(assigned.Peek(), 10);

    Stack<std::string> words;
    words.Push("one");
    words.Push("three");
    EXPECT_EQ(words.Peek(), "three");

    Stack<std::string> longWords = words.Where(IsLongWord);
    EXPECT_EQ(longWords.GetCount(), 1);
    EXPECT_EQ(longWords.Peek(), "three");

    Stack<int> lengths = words.Map<int>(StringLength);
    EXPECT_EQ(lengths.Get(0), 3);
    EXPECT_EQ(lengths.Get(1), 5);
}

// очередь: добавить в конец, забрать из начала
TEST(QueueTests, EnqueueDequeueAndCommonOperations) {
    Queue<int> queue;
    EXPECT_TRUE(queue.IsEmpty());
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    EXPECT_EQ(queue.GetCount(), 3);
    EXPECT_EQ(queue.Peek(), 1);
    EXPECT_EQ(queue.Dequeue(), 1);
    EXPECT_EQ(queue.Peek(), 2);

    Queue<int> mapped = queue.Map<int>(DoubleTestInt);
    EXPECT_EQ(mapped.Get(0), 4);
    EXPECT_EQ(mapped.Get(1), 6);

    Queue<int> other;
    other.Enqueue(4);
    Queue<int> joined = queue + other;
    EXPECT_EQ(joined.GetCount(), 3);
    EXPECT_EQ(joined.Get(2), 4);

    Queue<int> sub = joined.GetSubqueue(1, 2);
    EXPECT_EQ(sub.Get(0), 3);
    EXPECT_EQ(sub.Get(1), 4);

    Queue<int> empty;
    EXPECT_THROW(empty.Dequeue(), std::out_of_range);
    EXPECT_THROW(empty.Peek(), std::out_of_range);
}

// дек: работа с началом и концом
TEST(DequeTests, PushPopBothSidesAndCommonOperations) {
    Deque<int> deque;
    EXPECT_TRUE(deque.IsEmpty());
    deque.PushBack(2);
    deque.PushFront(1);
    deque.PushBack(3);
    deque.PushFront(0);

    EXPECT_EQ(deque.GetCount(), 4);
    EXPECT_EQ(deque.PeekFront(), 0);
    EXPECT_EQ(deque.PeekBack(), 3);
    EXPECT_EQ(deque.PopFront(), 0);
    EXPECT_EQ(deque.PopBack(), 3);
    EXPECT_EQ(deque.Get(0), 1);
    EXPECT_EQ(deque.Get(1), 2);

    Deque<int> filtered = deque.Where(IsEvenTestInt);
    EXPECT_EQ(filtered.GetCount(), 1);
    EXPECT_EQ(filtered.PeekFront(), 2);

    Deque<int> other;
    other.PushBack(5);
    Deque<int> joined = deque + other;
    EXPECT_EQ(joined.GetCount(), 3);
    EXPECT_EQ(joined.PeekBack(), 5);

    Deque<int> empty;
    EXPECT_THROW(empty.PopFront(), std::out_of_range);
    EXPECT_THROW(empty.PopBack(), std::out_of_range);
    EXPECT_THROW(empty.PeekFront(), std::out_of_range);
    EXPECT_THROW(empty.PeekBack(), std::out_of_range);
}
