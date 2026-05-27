#pragma once

#include <cstddef>

template <class T>
class ICollection {
public:
    virtual ~ICollection() {}

    virtual T Get(std::size_t index) const = 0;
    virtual std::size_t GetCount() const = 0;
};
