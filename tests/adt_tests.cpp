#include <gtest/gtest.h>

#include <string>

#include "Deque.hpp"
#include "PriorityQueue.hpp"
#include "Queue.hpp"
#include "Stack.hpp"

TEST(StackTests, PushPopPeekAndConcat) {
    Stack<int> stack;
    EXPECT_TRUE(stack.IsEmpty());

    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    EXPECT_EQ(stack.GetCount(), 3);
    EXPECT_EQ(stack.Get(0), 30);
    EXPECT_EQ(stack.Peek(), 30);
    EXPECT_EQ(stack.Pop(), 30);
    EXPECT_EQ(stack.Peek(), 20);

    Stack<int> other;
    other.Push(40);
    other.Push(50);

    Stack<int> joined = stack.Concat(other);
    EXPECT_EQ(joined.GetCount(), 4);
    EXPECT_EQ(joined.Get(0), 50);
    EXPECT_EQ(joined.Get(3), 10);

    Stack<int> empty;
    EXPECT_THROW(empty.Pop(), std::out_of_range);
    EXPECT_THROW(empty.Peek(), std::out_of_range);
}

TEST(StackTests, WorksWithString) {
    Stack<std::string> words;
    words.Push("one");
    words.Push("two");

    EXPECT_EQ(words.GetCount(), 2);
    EXPECT_EQ(words.Peek(), "two");
}

TEST(QueueTests, EnqueueDequeuePeekAndConcat) {
    Queue<int> queue;
    EXPECT_TRUE(queue.IsEmpty());

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    EXPECT_EQ(queue.GetCount(), 3);
    EXPECT_EQ(queue.Get(1), 2);
    EXPECT_EQ(queue.Peek(), 1);
    EXPECT_EQ(queue.Dequeue(), 1);
    EXPECT_EQ(queue.Peek(), 2);

    Queue<int> other;
    other.Enqueue(4);

    Queue<int> joined = queue.Concat(other);
    EXPECT_EQ(joined.GetCount(), 3);
    EXPECT_EQ(joined.Get(0), 2);
    EXPECT_EQ(joined.Get(2), 4);

    Queue<int> empty;
    EXPECT_THROW(empty.Dequeue(), std::out_of_range);
    EXPECT_THROW(empty.Peek(), std::out_of_range);
}

TEST(DequeTests, PushPopBothSidesAndConcat) {
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

    Deque<int> other;
    other.PushBack(5);
    Deque<int> joined = deque.Concat(other);
    EXPECT_EQ(joined.GetCount(), 3);
    EXPECT_EQ(joined.PeekBack(), 5);

    Deque<int> empty;
    EXPECT_THROW(empty.PopFront(), std::out_of_range);
    EXPECT_THROW(empty.PopBack(), std::out_of_range);
    EXPECT_THROW(empty.PeekFront(), std::out_of_range);
    EXPECT_THROW(empty.PeekBack(), std::out_of_range);
}

TEST(PriorityQueueTests, EnqueueDequeueByPriority) {
    PriorityQueue<std::string> queue;
    EXPECT_TRUE(queue.IsEmpty());

    queue.Enqueue("low", 1);
    queue.Enqueue("high", 10);
    queue.Enqueue("middle", 5);
    queue.Enqueue("high second", 10);

    EXPECT_EQ(queue.GetCount(), 4);
    EXPECT_EQ(queue.Peek(), "high");
    EXPECT_EQ(queue.PeekPriority(), 10);
    EXPECT_EQ(queue.Dequeue(), "high");
    EXPECT_EQ(queue.Dequeue(), "high second");
    EXPECT_EQ(queue.Dequeue(), "middle");
    EXPECT_EQ(queue.Dequeue(), "low");
    EXPECT_TRUE(queue.IsEmpty());

    EXPECT_THROW(queue.Dequeue(), std::out_of_range);
    EXPECT_THROW(queue.Peek(), std::out_of_range);
    EXPECT_THROW(queue.PeekPriority(), std::out_of_range);
}

TEST(PriorityQueueTests, ConcatKeepsPriorityOrder) {
    PriorityQueue<int> first;
    first.Enqueue(10, 1);
    first.Enqueue(20, 3);

    PriorityQueue<int> second;
    second.Enqueue(30, 2);
    second.Enqueue(40, 4);

    PriorityQueue<int> joined = first.Concat(second);
    EXPECT_EQ(joined.GetCount(), 4);
    EXPECT_EQ(joined.Get(0).value, 40);
    EXPECT_EQ(joined.Get(1).value, 20);
    EXPECT_EQ(joined.Get(2).value, 30);
    EXPECT_EQ(joined.Get(3).value, 10);
}
