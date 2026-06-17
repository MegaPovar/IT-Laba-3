#include <gtest/gtest.h>

#include <string>

#include "ArraySequence.hpp"
#include "Deque.hpp"
#include "InversionAlgorithms.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "NumberAlgorithms.hpp"
#include "Queue.hpp"
#include "SequenceAlgorithms.hpp"
#include "Stack.hpp"

static int SquareTestInt(int value) {
    return value * value;
}

static bool IsEvenTestInt(int value) {
    return value % 2 == 0;
}

static int SumTestInt(int sum, int value) {
    return sum + value;
}

static int AddIndexTestInt(int value, int index) {
    return value + index;
}

static bool GreaterThanThree(int value) {
    return value > 3;
}

static bool GreaterThanTen(int value) {
    return value > 10;
}

static int DoubleTestInt(int value) {
    return value * 2;
}

static bool AtLeastTwenty(int value) {
    return value >= 20;
}

static bool IsLongWord(std::string value) {
    return value.size() > 3;
}

static int StringLength(std::string value) {
    return static_cast<int>(value.size());
}

static bool LessTestInt(int left, int right) {
    return left < right;
}

TEST(DynamicArrayTests, BasicOperationsAndErrors) {
    int source[] = {1, 2, 3};
    DynamicArray<int> array(source, 3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(1), 2);

    array.Set(1, 20);
    EXPECT_EQ(array[1], 20);

    array.Resize(5);
    EXPECT_EQ(array.GetSize(), 5);
    array.Set(4, 50);
    EXPECT_EQ(array.Get(4), 50);

    EXPECT_THROW(array.Get(10), IndexOutOfRange);

    array.Resize(0);
    EXPECT_EQ(array.GetSize(), 0);
    EXPECT_THROW(DynamicArray<int> invalid(-1), InvalidArgument);
}

TEST(LinkedListTests, BasicOperationsAndErrors) {
    int source[] = {2, 3};
    LinkedList<int> list(source, 2);
    list.Prepend(1);
    list.Append(4);
    list.InsertAt(10, 2);

    EXPECT_EQ(list.GetLength(), 5);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 4);
    EXPECT_EQ(list.Get(2), 10);

    LinkedList<int>* sub = list.GetSubList(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    delete sub;

    LinkedList<int> copy(list);
    EXPECT_EQ(copy.GetLength(), list.GetLength());
    EXPECT_EQ(copy.Get(2), 10);

    LinkedList<int> empty;
    EXPECT_THROW(empty.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(empty.GetLast(), IndexOutOfRange);
    EXPECT_THROW(empty.InsertAt(1, 2), IndexOutOfRange);
}

TEST(SequenceTests, MutableAndImmutableBehavior) {
    int source[] = {1, 2, 3};
    MutableArraySequence<int> mutableArray(source, 3);
    Sequence<int>* same = mutableArray.Append(4);
    EXPECT_EQ(same, &mutableArray);
    EXPECT_EQ(mutableArray.GetLast(), 4);

    ImmutableArraySequence<int> immutableArray(source, 3);
    Sequence<int>* changed = immutableArray.Append(4);
    EXPECT_EQ(immutableArray.GetLength(), 3);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->GetLast(), 4);
    delete changed;

    MutableListSequence<int> listSeq(source, 3);
    listSeq.Prepend(0);
    EXPECT_EQ(listSeq.GetFirst(), 0);
    EXPECT_EQ(listSeq[2], 2);

    Sequence<int>* listSub = listSeq.GetSubsequence(1, 2);
    EXPECT_EQ(listSub->GetLength(), 2);
    EXPECT_EQ(listSub->Get(0), 1);
    EXPECT_EQ(listSub->Get(1), 2);
    delete listSub;

    EXPECT_THROW(delete mutableArray.GetSubsequence(-1, 1), IndexOutOfRange);
    EXPECT_THROW(delete mutableArray.GetSubsequence(2, 1), InvalidArgument);
}

TEST(SequenceAlgorithmTests, MapWhereReduceSliceZip) {
    int source[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> sequence(source, 5);

    Sequence<int>* squares = sequence.Map<int>(SquareTestInt);
    EXPECT_EQ(squares->Get(2), 9);
    delete squares;

    Sequence<int>* even = sequence.Where(IsEvenTestInt);
    EXPECT_EQ(even->GetLength(), 2);
    EXPECT_EQ(even->Get(1), 4);
    delete even;

    EXPECT_EQ(sequence.Reduce<int>(SumTestInt, 0), 15);

    Sequence<int>* indexed = sequence.MapIndexed<int>(AddIndexTestInt);
    EXPECT_EQ(indexed->Get(3), 7);
    delete indexed;

    Sequence<int>* inserted = new MutableArraySequence<int>();
    inserted->Append(9)->Append(10);
    Sequence<int>* sliced = sequence.Slice(1, 2, inserted);
    EXPECT_EQ(sliced->GetLength(), 5);
    EXPECT_EQ(sliced->Get(0), 1);
    EXPECT_EQ(sliced->Get(1), 9);
    EXPECT_EQ(sliced->Get(2), 10);
    EXPECT_EQ(sliced->Get(3), 4);
    delete inserted;
    delete sliced;

    Option<int> found = sequence.TryGetFirst(GreaterThanThree);
    EXPECT_TRUE(found.HasValue());
    EXPECT_EQ(found.Value(), 4);

    Option<int> missing = sequence.TryGetLast(GreaterThanTen);
    EXPECT_FALSE(missing.HasValue());
    EXPECT_THROW(missing.Value(), std::logic_error);

    Sequence<int>* none = sequence.Where(GreaterThanTen);
    EXPECT_EQ(none->GetLength(), 0);
    delete none;

    EXPECT_THROW(delete sequence.Slice(0, -1), InvalidArgument);
    EXPECT_THROW(delete sequence.Slice(4, 2), IndexOutOfRange);

    int shortSource[] = {7, 8};
    MutableArraySequence<int> shortSequence(shortSource, 2);
    Sequence<std::pair<int, int> >* zipped = Zip<int>(&sequence, &shortSequence);
    EXPECT_EQ(zipped->GetLength(), 2);
    EXPECT_EQ(zipped->Get(0).first, 1);
    EXPECT_EQ(zipped->Get(0).second, 7);
    delete zipped;
}

TEST(LinearContainerTests, CommonBehaviorThroughStack) {
    int source[] = {1, 2, 3, 2, 3, 4};
    Stack<int> stack(source, 6);

    EXPECT_EQ(stack.GetCount(), 6);
    EXPECT_EQ(stack.Get(0), 1);

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
    EXPECT_THROW(iterator.Get(), IndexOutOfRange);
}

TEST(StackTests, PushPopMapWhereReduceAndOperators) {
    Stack<int> stack;
    EXPECT_TRUE(stack.IsEmpty());
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    EXPECT_EQ(stack.GetCount(), 3);
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
    EXPECT_THROW(empty.Pop(), IndexOutOfRange);
    EXPECT_THROW(empty.Peek(), IndexOutOfRange);
    EXPECT_THROW(empty.Get(0), IndexOutOfRange);
    EXPECT_THROW(stack.GetSubstack(-1, 1), IndexOutOfRange);
    EXPECT_THROW(stack.GetSubstack(0, 10), IndexOutOfRange);
    EXPECT_THROW(stack.GetSubstack(1, 0), InvalidArgument);
}

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
    EXPECT_THROW(empty.Dequeue(), IndexOutOfRange);
    EXPECT_THROW(empty.Peek(), IndexOutOfRange);
}

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
    EXPECT_THROW(empty.PopFront(), IndexOutOfRange);
    EXPECT_THROW(empty.PopBack(), IndexOutOfRange);
    EXPECT_THROW(empty.PeekFront(), IndexOutOfRange);
    EXPECT_THROW(empty.PeekBack(), IndexOutOfRange);
}

TEST(InversionTests, LoopsAndMapReduce) {
    int source[] = {3, 1, 2, 5, 4};
    MutableArraySequence<int> sequence(source, 5);
    std::function<bool(int, int)> less = LessTestInt;

    EXPECT_EQ(CountInversionsLoops<int>(sequence, less), 3);
    EXPECT_EQ(CountInversionsMapReduce<int>(sequence, less), 3);

    int sortedSource[] = {1, 2, 3, 4};
    MutableArraySequence<int> sorted(sortedSource, 4);
    EXPECT_EQ(CountInversionsLoops<int>(sorted, less), 0);
    EXPECT_EQ(CountInversionsMapReduce<int>(sorted, less), 0);

    int reversedSource[] = {4, 3, 2, 1};
    MutableListSequence<int> reversed(reversedSource, 4);
    EXPECT_EQ(CountInversionsLoops<int>(reversed, less), 6);
    EXPECT_EQ(CountInversionsMapReduce<int>(reversed, less), 6);

    int duplicatesSource[] = {2, 2, 1};
    MutableArraySequence<int> duplicates(duplicatesSource, 3);
    EXPECT_EQ(CountInversionsLoops<int>(duplicates, less), 2);
    EXPECT_EQ(CountInversionsMapReduce<int>(duplicates, less), 2);

    MutableArraySequence<int> empty;
    EXPECT_EQ(CountInversionsLoops<int>(empty, less), 0);
    EXPECT_EQ(CountInversionsMapReduce<int>(empty, less), 0);
}

TEST(NumberAlgorithmTests, RangePrimesAndFactorization) {
    Sequence<int>* range = Range(3, 6);
    EXPECT_EQ(range->GetLength(), 4);
    EXPECT_EQ(range->Get(0), 3);
    EXPECT_EQ(range->Get(3), 6);
    delete range;

    Sequence<int>* oneNumber = Range(5, 5);
    EXPECT_EQ(oneNumber->GetLength(), 1);
    EXPECT_EQ(oneNumber->GetFirst(), 5);
    delete oneNumber;

    EXPECT_FALSE(IsPrime(-3));
    EXPECT_FALSE(IsPrime(0));
    EXPECT_FALSE(IsPrime(1));
    EXPECT_TRUE(IsPrime(2));
    EXPECT_TRUE(IsPrime(97));
    EXPECT_FALSE(IsPrime(100));

    Sequence<int>* primes = PrimesInRange(1, 10);
    EXPECT_EQ(primes->GetLength(), 4);
    EXPECT_EQ(primes->Get(0), 2);
    EXPECT_EQ(primes->Get(3), 7);
    delete primes;

    Sequence<int>* noPrimes = PrimesInRange(14, 16);
    EXPECT_EQ(noPrimes->GetLength(), 0);
    delete noPrimes;

    Sequence<int>* divisors = Factorize(12);
    EXPECT_EQ(divisors->GetLength(), 6);
    EXPECT_EQ(divisors->Get(0), 1);
    EXPECT_EQ(divisors->Get(1), 12);
    delete divisors;

    Sequence<int>* primeFactors = PrimeFactorization(84);
    EXPECT_EQ(primeFactors->GetLength(), 4);
    EXPECT_EQ(primeFactors->Get(0), 2);
    EXPECT_EQ(primeFactors->Get(1), 2);
    EXPECT_EQ(primeFactors->Get(2), 3);
    EXPECT_EQ(primeFactors->Get(3), 7);
    delete primeFactors;

    Sequence<int>* primeValue = PrimeFactorization(13);
    EXPECT_EQ(primeValue->GetLength(), 1);
    EXPECT_EQ(primeValue->GetFirst(), 13);
    delete primeValue;

    EXPECT_THROW(delete Range(5, 1), InvalidArgument);
    EXPECT_THROW(delete Factorize(0), InvalidArgument);
    EXPECT_THROW(delete Factorize(-10), InvalidArgument);
    EXPECT_THROW(delete PrimeFactorization(1), InvalidArgument);
}
