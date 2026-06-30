#pragma once

#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

    void CheckIndex(int index) const { 
        if (index < 0 || index >= size) {
            throw std::out_of_range("DynamicArray index is out of range");
        }
    }

public:
    DynamicArray() : data(nullptr), size(0) {}

    DynamicArray(T* items, int count) : data(nullptr), size(count) {
        if (count < 0) {
            throw std::invalid_argument("DynamicArray size cannot be negative");
        }
        if (count == 0) {
            data = nullptr;
        } else {
            data = new T[count];
        }
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    explicit DynamicArray(int size) : data(nullptr), size(size) {
        if (size < 0) {
            throw std::invalid_argument("DynamicArray size cannot be negative");
        }
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[size]();
        }
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) : data(nullptr), size(dynamicArray.size) {
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[size];
        }
        for (int i = 0; i < size; ++i) {
            data[i] = dynamicArray.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        T* newData;
        if (other.size == 0) {
            newData = nullptr;
        } else {
            newData = new T[other.size];
        }
        for (int i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        size = other.size;
        return *this;
    }

    ~DynamicArray() {
        delete[] data; 
    }

    T Get(int index) const { 
        CheckIndex(index);
        return data[index];
    }

    int GetSize() const {
        return size;
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("DynamicArray size cannot be negative");
        }
        T* newData;
        if (newSize == 0) {
            newData = nullptr;
        } else {
            newData = new T[newSize]();
        }

        int copyCount;
        if (size < newSize) {
            copyCount = size;
        } else {
            copyCount = newSize;
        }
        for (int i = 0; i < copyCount; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData; 
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
