#pragma once
#include <cstddef>
#include <memory>


template <typename T>
class ListElement {
public:
    T value;
    std::unique_ptr<ListElement<T>> next;

    ListElement(T _value);

    bool isEnd();
    void count(size_t* counter);
};

template <typename T>
class List {
    std::unique_ptr<ListElement<T>> firstElement;

public:
    List(T _value);
    ~List();
    
    void push_back(T _value);
    void push_front(T _value);
    T back();
    T front();

    bool empty();
    size_t size();

    ListElement<T>* begin();
    ListElement<T>* end();
};

#include "list.hpp"

template <typename T>
ListElement<T>::ListElement(T _value) : value(_value)
{}

template <typename T>
bool ListElement<T>::isEnd()
{
    if(next) return true;
    return false;
}

template <typename T>
void ListElement<T>::count(size_t* counter)
{
    if(next){
        (*counter)++;
        next->count(counter);
    }
}

template <typename T>
List<T>::List(T _value)
{
    firstElement = std::make_unique<ListElement<T>>(_value);
}

// template <typename T>
// List<T>::~List()
// {
//     delete firstElement;
// }

template <typename T>
bool List<T>::empty()
{
    if(!firstElement) return true;
    return false;
}

template <typename T>
size_t List<T>::size()
{
    size_t counter=0;
    if(firstElement) {
        counter++;
        firstElement->count(&counter);
    }
    return counter;
}

