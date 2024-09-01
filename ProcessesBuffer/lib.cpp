#include "lib.hpp"

int get_random_buffer(int max, const std::vector<int>& excluded)
{
    std::vector<int> available_numbers;
    for (int i = 0; i <= max; ++i)
    {
        if (std::find(excluded.begin(), excluded.end(), i) == excluded.end())
        {
            available_numbers.push_back(i);
        }
    }
    if (available_numbers.empty())
    {
        return -2;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, available_numbers.size() - 1);

    return available_numbers[dist(gen)];
}

void clear_times(int buff_num, std::vector<int> (&vecArray)[8])
{
    for (int i = 0; i < 8; ++i) {
        vecArray[i].erase(std::remove_if(vecArray[i].begin(), vecArray[i].end(), [buff_num](int num) {
            return num == buff_num;}), vecArray[i].end());
    }
}

void print_buffers(SharedMemory* shared_memory, std::string name, int buffer_index) {

    for(int i=0; i<BUFFER_COUNT; i++)
    {
        std::cout << "buff " << i << ": [";
        for (auto v : shared_memory->buffers[i].values)
            std::cout << v << ",";
        std::cout << "\b]";
    }
    

    // std::cout << "   ###   buff 1: [";
    // for (int i = 0; i < shared_memory->buffers[1].count; ++i) {
    //     std::cout << shared_memory->buffers[1].buffer[(shared_memory->buffers[1].out + i) % BUFFER_SIZE];
    //     if (i < shared_memory->buffers[1].count - 1) std::cout << ", ";
    // }
    // std::cout << "]";

    // std::cout << "   ###   buff 2: [";
    // for (int i = 0; i < shared_memory->buffers[2].count; ++i) {
    //     std::cout << shared_memory->buffers[2].buffer[(shared_memory->buffers[2].out + i) % BUFFER_SIZE];
    //     if (i < shared_memory->buffers[2].count - 1) std::cout << ", ";
    // }
    // std::cout << "]";

    // if (action != nullptr) {
    //     std::cout << " <- " << action << " buffer " << buffer_index;
    // }
    std::cout << " <- " << name << " buffer " << buffer_index;

    // std::cout << std::endl;
    // std::string nameP = "";
    // std::string nameC = "";
    // if(name=="Producer"){
    //     nameP = "<- " + name;
    // } else {
    //     nameC = name + " ->";
    // }
    // std::cout << "\n Buff: " << buffIndex<<  " [size:" << values.size() << "]    ### " << nameC << " [";
    // for (auto v : values)
    //     std::cout << v << ",";
    // std::cout << "] " << nameP << " ###    Who has read: ";

}