#include "hash.hpp"

HashTable::HashTable(int _modulo): modulo(_modulo), tab(_modulo){} 

int HashTable::hashfunction(std::string word)
    {
        int sum = 0;
        for(int i=0; i<word.size(); i++)
        {
            sum += word[i];
        }
        return sum % modulo;
    }
    
void HashTable::insert(std::string word)
{
    int hash = hashfunction(word);
    if(!std::get<0>(find(word)))
    {
        tab[hash].push_back(word);
        std::cout << "New word has been successfully added: " << word << std::endl;
    }
}

std::tuple<bool, int, int> HashTable::find(std::string word)
{
    std::tuple<bool, int, int> tuple;
    int hash = hashfunction(word);
    std::get<1>(tuple) = hash;
    for(int i=0;i<tab[hash].size();i++)
    {
        if(tab[hash][i] == word)
        {
            std::cout << "There is already this word in table: " << word << std::endl;
            std::get<0>(tuple) = true;
            std::get<2>(tuple) = i;
            return tuple;
        }
    }
    std::cout << "Word: " << word << " not found" << std::endl;
    std::get<0>(tuple) = false;
    std::get<1>(tuple) = -1;
    std::get<2>(tuple) = -1;
    return tuple;
}

void HashTable::delete_(std::string word)
{
    std::tuple<bool, int, int> tuple = find(word);
    if(std::get<0>(tuple))
    {
        tab[std::get<1>(tuple)].erase(tab[std::get<1>(tuple)].begin() + std::get<2>(tuple));
        std::cout << "Word " << word << " has been deleted." << std::endl;
    }
}

size_t HashTable::get_size()
{
    int sum = 0;
    for(int i=0;i<modulo;i++)
    {
        sum += tab[i].size();
    }
    return sum;
}

size_t HashTable::get_size_of_cup(int cup_num)
{
    return tab[cup_num].size();
}