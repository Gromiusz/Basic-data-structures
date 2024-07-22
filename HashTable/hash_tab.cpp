#include <iostream>
#include "hash.hpp"

int main(){
    HashTable hashtable(10);
    hashtable.insert("ab");
    auto [a,b,c] = hashtable.find("ab");
}