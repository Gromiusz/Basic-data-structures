#pragma once
#include <string>
#include <iostream>
#include <tuple>
#include <vector>

class HashTable
{
    int modulo;
    std::vector<std::vector<std::string>> tab;
    //std::vector<std::pair<int, std::string>> tab[10];
    int hashfunction(std::string word);
    
public:

    HashTable(int _modulo); 
    void insert(std::string word);
    void delete_(std::string word);
    std::tuple<bool, int, int> find(std::string word);
    size_t get_size();
    size_t get_size_of_cup(int cup_num);
};