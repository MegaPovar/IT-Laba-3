#pragma once

template <class T>
class ICollection {
public:
    virtual ~ICollection() {} //для удаления через указатель

    virtual T Get(int index) const = 0; // получить элемент по номеру
    virtual int GetCount() const = 0; // количество элементов
};
