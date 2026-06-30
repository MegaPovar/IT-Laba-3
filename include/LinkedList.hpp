#pragma once

#include <stdexcept>

template <class T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& value) : value(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

    void CheckIndex(int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("LinkedList index is out of range");
        }
    }

    void CheckNotEmpty() const {
        if (length == 0) {
            throw std::out_of_range("LinkedList is empty");
        }
    }

    Node* GetNode(int index) const {
        CheckIndex(index);
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

public:
    class Iterator {
    private:
        const Node* current;

    public:
        explicit Iterator(const Node* start) : current(start) {}

        bool HasValue() const {
            return current != nullptr;
        }

        const T& Get() const {
            if (current == nullptr) {
                throw std::out_of_range("Iterator is out of range");
            }
            return current->value;
        }

        void MoveNext() {
            if (current != nullptr) {
                current = current->next;
            }
        }
    };

    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(T* items, int count) : LinkedList() {
        if (count < 0) {
            throw std::invalid_argument("LinkedList length cannot be negative");
        }
        if (items == nullptr && count > 0) {
            throw std::invalid_argument("LinkedList source cannot be null");
        }
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& list) : LinkedList() {
        Iterator iterator = list.Begin();
        while (iterator.HasValue()) {
            Append(iterator.Get());
            iterator.MoveNext();
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        Iterator iterator = other.Begin();
        while (iterator.HasValue()) {
            Append(iterator.Get());
            iterator.MoveNext();
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    const T& GetFirst() const {
        CheckNotEmpty();
        return head->value;
    }

    const T& GetLast() const {
        CheckNotEmpty();
        return tail->value;
    }

    const T& Get(int index) const {
        return GetNode(index)->value;
    }

    Iterator Begin() const {
        return Iterator(head);
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        CheckIndex(startIndex);
        CheckIndex(endIndex);
        if (startIndex > endIndex) {
            throw std::invalid_argument("startIndex cannot be greater than endIndex");
        }
        LinkedList<T>* result = new LinkedList<T>();
        Iterator iterator = Begin();
        for (int i = 0; i < startIndex; ++i) {
            iterator.MoveNext();
        }
        for (int i = startIndex; i <= endIndex; ++i) {
            result->Append(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    int GetLength() const {
        return length;
    }

    void Append(const T& item) {
        Node* node = new Node(item);
        if (length == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    void Prepend(const T& item) {
        Node* node = new Node(item);
        node->next = head;
        head = node;
        if (length == 0) {
            tail = node;
        }
        ++length;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > length) {
            throw std::out_of_range("LinkedList insert index is out of range");
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == length) {
            Append(item);
            return;
        }
        Node* previous = GetNode(index - 1);
        Node* node = new Node(item);
        node->next = previous->next;
        previous->next = node;
        ++length;
    }

    T RemoveFirst() {
        CheckNotEmpty();

        Node* oldHead = head;
        T value = oldHead->value;
        head = head->next;
        delete oldHead;
        --length;

        if (length == 0) {
            tail = nullptr;
        }

        return value;
    }

    T RemoveLast() {
        CheckNotEmpty();

        if (length == 1) {
            return RemoveFirst();
        }

        Node* previous = GetNode(length - 2);
        Node* oldTail = tail;
        T value = oldTail->value;

        delete oldTail;
        tail = previous;
        tail->next = nullptr;
        --length;

        return value;
    }

    LinkedList<T>* Concat(const LinkedList<T>& list) const {
        LinkedList<T>* result = new LinkedList<T>(*this);
        Iterator iterator = list.Begin();
        while (iterator.HasValue()) {
            result->Append(iterator.Get());
            iterator.MoveNext();
        }
        return result;
    }

    T& operator[](int index) {
        return GetNode(index)->value;
    }

    const T& operator[](int index) const {
        return GetNode(index)->value;
    }
};
