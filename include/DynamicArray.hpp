#pragma once

#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

    void CheckIndex(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("DynamicArray index is out of range");
        }
    }

    void Reserve(int newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }

        T* newData = new T[newCapacity]();
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(T* items, int count) : data(nullptr), size(0), capacity(0) {
        if (count < 0) {
            throw std::invalid_argument("DynamicArray size cannot be negative");
        }
        if (items == nullptr && count > 0) {
            throw std::invalid_argument("DynamicArray source cannot be null");
        }

        Resize(count);
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    explicit DynamicArray(int size) : data(nullptr), size(0), capacity(0) {
        Resize(size);
    }

    DynamicArray(const DynamicArray<T>& other) : data(nullptr), size(0), capacity(0) {
        Resize(other.size);
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }

        DynamicArray<T> copy(other);
        T* oldData = data;
        data = copy.data;
        size = copy.size;
        capacity = copy.capacity;

        copy.data = oldData;
        copy.size = 0;
        copy.capacity = 0;
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    const T& Get(int index) const {
        CheckIndex(index);
        return data[index];
    }

    int GetSize() const {
        return size;
    }

    int GetCapacity() const {
        return capacity;
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("DynamicArray size cannot be negative");
        }

        if (newSize > capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity;
            while (newCapacity < newSize) {
                newCapacity *= 2;
            }
            Reserve(newCapacity);
        }

        for (int i = size; i < newSize; ++i) {
            data[i] = T();
        }
        size = newSize;
    }

    T& operator[](int index) {
        CheckIndex(index);
        return data[index];
    }

    const T& operator[](int index) const {
        CheckIndex(index);
        return data[index];
    }
};
