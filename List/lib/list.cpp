// #include "list.hpp"

// template <typename T>
// ListElement<T>::ListElement(T _value) : value(_value)
// {}

// template <typename T>
// bool ListElement<T>::isEnd()
// {
//     if(next) return true;
//     return false;
// }

// template <typename T>
// void ListElement<T>::count(size_t* counter)
// {
//     if(next){
//         (*counter)++;
//         next->count(counter);
//     }
// }

// template <typename T>
// List<T>::List(T _value)
// {
//     firstElement = std::make_unique<ListElement<T>>(_value);
// }

// // template <typename T>
// // List<T>::~List()
// // {
// //     delete firstElement;
// // }

// template <typename T>
// bool List<T>::empty()
// {
//     if(!firstElement) return true;
//     return false;
// }

// template <typename T>
// size_t List<T>::size()
// {
//     size_t counter=0;
//     if(firstElement) {
//         counter++;
//         firstElement->count(&counter);
//     }
//     return counter;
// }

