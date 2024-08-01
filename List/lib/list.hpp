#pragma once
#include <cstddef>
#include <memory>


template <typename T>
class ListElement {
public:
    T value;
    std::shared_ptr<ListElement<T>> next;

    ListElement(T _value);
    void count(size_t* counter);
    std::shared_ptr<ListElement<T>> end();
};

template <typename T>
class List {
    std::shared_ptr<ListElement<T>> firstElement;

public:
    List(T _value);
    ~List();
    
    void push_back(T _value);
    void push_front(T _value);
    T back();
    T front();

    bool empty();
    size_t size();

    std::shared_ptr<ListElement<T>> begin();
    std::shared_ptr<ListElement<T>> end();

    class Iterator {
        std::shared_ptr<ListElement<T>> current_pointer;

    public:
        Iterator& operator++() {
            if(current_pointer->next) {
                current_pointer = current_pointer->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator copy = *this;
            ++(*this);
            return copy;
        }

        bool operator==(const Iterator& other) const {return current_pointer == other.current_pointer};
        bool operator!=(const Iterator& other) const {return current_pointer != other.current_pointer};
    }

};


template <typename T>
std::shared_ptr<ListElement<T>> ListElement<T>::end()
{
    if(next) return next->end();
    return pointerOnMe;
}

template <typename T>
std::shared_ptr<ListElement<T>> List<T>::begin()
{
    return next;
}

template <typename T>
std::shared_ptr<ListElement<T>> List<T>::end()
{

}

template <typename T>
ListElement<T>::ListElement(T _value) : value(_value)
{}

// template <typename T>
// bool ListElement<T>::isEnd()
// {
//     if(next) return true;
//     return false;
// }

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

