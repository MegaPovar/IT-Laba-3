#include <gtest/gtest.h>

#include "ArraySequence.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"
#include "test_helpers.hpp"

// DynamicArray: размер, доступ, resize и ошибки
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

    EXPECT_THROW(array.Get(10), std::out_of_range);

    array.Resize(0);
    EXPECT_EQ(array.GetSize(), 0);
    EXPECT_GE(array.GetCapacity(), 5);
    EXPECT_THROW(DynamicArray<int> invalid(-1), std::invalid_argument);
    EXPECT_THROW(DynamicArray<int> nullSource(nullptr, 3), std::invalid_argument);
}

// LinkedList: добавление, вставка и кусок списка
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
    EXPECT_THROW(empty.GetFirst(), std::out_of_range);
    EXPECT_THROW(empty.GetLast(), std::out_of_range);
    EXPECT_THROW(empty.InsertAt(1, 2), std::out_of_range);
    EXPECT_THROW(LinkedList<int> nullSource(nullptr, 2), std::invalid_argument);
}

// sequence на массиве и списке, mutable/immutable
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

    EXPECT_THROW(delete mutableArray.GetSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(delete mutableArray.GetSubsequence(2, 1), std::invalid_argument);
}

TEST(SequenceTests, SelfConcatWorksForMutableArrayAndList) {
    int source[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(source, 3);
    Sequence<int>* arrayResult = arraySequence.Concat(arraySequence);
    EXPECT_EQ(arrayResult, &arraySequence);
    EXPECT_EQ(arraySequence.GetLength(), 6);
    EXPECT_EQ(arraySequence.Get(0), 1);
    EXPECT_EQ(arraySequence.Get(3), 1);
    EXPECT_EQ(arraySequence.Get(5), 3);

    MutableListSequence<int> listSequence(source, 3);
    Sequence<int>* listResult = listSequence.Concat(listSequence);
    EXPECT_EQ(listResult, &listSequence);
    EXPECT_EQ(listSequence.GetLength(), 6);
    EXPECT_EQ(listSequence.Get(0), 1);
    EXPECT_EQ(listSequence.Get(3), 1);
    EXPECT_EQ(listSequence.Get(5), 3);
}

// map, where, reduce и остальные методы sequence
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

    EXPECT_THROW(delete sequence.Slice(0, -1), std::invalid_argument);
    EXPECT_THROW(delete sequence.Slice(4, 2), std::out_of_range);

    int shortSource[] = {7, 8};
    MutableArraySequence<int> shortSequence(shortSource, 2);
    Sequence<std::pair<int, int> >* zipped = Zip<int>(&sequence, &shortSequence);
    EXPECT_EQ(zipped->GetLength(), 2);
    EXPECT_EQ(zipped->Get(0).first, 1);
    EXPECT_EQ(zipped->Get(0).second, 7);
    delete zipped;

    EXPECT_THROW(delete sequence.Map<int>(std::function<int(int)>()), std::invalid_argument);
    EXPECT_THROW(delete sequence.Where(std::function<bool(int)>()), std::invalid_argument);
    EXPECT_THROW(sequence.Reduce<int>(std::function<int(int, int)>(), 0), std::invalid_argument);
    EXPECT_THROW(delete Zip<int>(nullptr, &shortSequence), std::invalid_argument);
}
