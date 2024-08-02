#pragma once
#include <cstddef>
#include <memory>

template <typename T>
class ListElement
{
public:
    T value;
    std::shared_ptr<ListElement<T>> next;

    ListElement(T _value);
    void count_size(size_t *counter);
    std::shared_ptr<ListElement<T>> end();
};

template <typename T>
class List
{
    std::shared_ptr<ListElement<T>> firstElement;

public:
    List() = default;
    List(T _value);
    void push_back(T _value);
    void push_front(T _value);
    T back();
    T front();
    bool empty();
    size_t size();

    class Iterator;
    Iterator begin();
    Iterator end();

    class Iterator
    {
        std::shared_ptr<ListElement<T>> current_pointer;

    public:
        Iterator(std::shared_ptr<ListElement<T>> ptr) : current_pointer(ptr) {}

        Iterator &operator++();
        Iterator operator++(int);
        T *operator->();
        T &operator*();

        bool operator==(const Iterator &other) const { return current_pointer == other.current_pointer; }
        bool operator!=(const Iterator &other) const { return current_pointer != other.current_pointer; }
        bool operator==(std::nullptr_t) const { return current_pointer == nullptr; }
        bool operator!=(std::nullptr_t) const { return current_pointer != nullptr; }
    };
};

template <typename T>
typename List<T>::Iterator &List<T>::Iterator::operator++()
{
    if(current_pointer->next)
    {
        current_pointer = current_pointer->next;
        return *this;
    }
    current_pointer = nullptr;
    return *this;
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int)
{
    Iterator copy = *this;
    ++(*this);
    return copy;
}

template <typename T>
T *List<T>::Iterator::operator->()
{
    return &current_pointer->value;
}

template <typename T>
T &List<T>::Iterator::operator*()
{
    return current_pointer->value;
}



template <typename T>
typename List<T>::Iterator List<T>::begin()
{
    return Iterator(firstElement);
}

template <typename T>
typename List<T>::Iterator List<T>::end()
{
    return Iterator(nullptr);
}

template <typename T>
void List<T>::push_front(T _value)
{
    if (!firstElement)
    {
        firstElement = std::make_shared<ListElement<T>>(_value);
        return;
    }
    auto pointer_cp = firstElement->next;
    firstElement = std::make_shared<ListElement<T>>(_value);
    firstElement->next = pointer_cp;
}

template <typename T>
void List<T>::push_back(T _value)
{
    if (!firstElement)
    {
        firstElement = std::make_shared<ListElement<T>>(_value);
        return;
    }
    std::shared_ptr<ListElement<T>> pointer = firstElement;
    while (pointer->next)
    {
        pointer = pointer->next;
    }
    pointer->next = std::make_shared<ListElement<T>>(_value);
}

template <typename T>
std::shared_ptr<ListElement<T>> ListElement<T>::end()
{
    if (next)
        return next->end();
    return std::shared_ptr<ListElement<T>>(this);
    ;
}

template <typename T>
ListElement<T>::ListElement(T _value) : value(_value)
{
    next = nullptr;
}

template <typename T>
void ListElement<T>::count_size(size_t *counter)
{
    if (next)
    {
        ++(*counter);
        next->count_size(counter);
    }
}

template <typename T>
List<T>::List(T _value)
{
    firstElement = std::make_shared<ListElement<T>>(_value);
}

template <typename T>
bool List<T>::empty()
{
    if (!firstElement)
        return true;
    return false;
}

template <typename T>
size_t List<T>::size()
{
    size_t counter = 0;
    if (firstElement)
    {
        ++counter;
        firstElement->count_size(&counter);
    }
    return counter;
}
