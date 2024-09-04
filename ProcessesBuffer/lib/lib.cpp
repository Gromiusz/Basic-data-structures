#include "lib.hpp"

int get_random_buffer(int max, const int* excluded, int excluded_count)
{
    std::vector<int> available_numbers;
    for (int i = 0; i <= max; ++i)
    {
        if (std::find(excluded, excluded + excluded_count, i) == excluded + excluded_count)
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

std::vector<int> get_possible_buffers(int max, const int* excluded, int excluded_count)
{
    std::vector<int> available_numbers;
    for (int i = 0; i <= max; ++i)
    {
        if (std::find(excluded, excluded + excluded_count, i) == excluded + excluded_count)
        {
            available_numbers.push_back(i);
        }
    }

    return available_numbers;
}

void clear_times(int buff_num, int which_buff_cons_have_read[CONSUMER_COUNT][MAX_CONSUMER_READ_BUFFS], int which_buff_cons_have_read_count[CONSUMER_COUNT])
{
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        int new_count = 0;
        for (int j = 0; j < which_buff_cons_have_read_count[i]; ++j) {
            if (which_buff_cons_have_read[i][j] != buff_num) {
                which_buff_cons_have_read[i][new_count++] = which_buff_cons_have_read[i][j];
            }
        }
        which_buff_cons_have_read_count[i] = new_count;
    }
}

void print_buffers(SharedMemory* shared_memory, std::string name, int buffer_index) {
    for(int i = 0; i < BUFFER_COUNT; i++) {
        std::cout << "buff " << i << ": [";
        for (int j = 0; j < shared_memory->buffers[i].count; j++) {
            std::cout << shared_memory->buffers[i].values[(shared_memory->buffers[i].out+j) % BUFFER_SIZE] << ",";
        }
        std::cout << "]";
        if(i != 2)
            std::cout << "   ###   ";
    }
    std::cout << " <- " << name << " buffer " << buffer_index << std::endl;
}

bool is_special_message(const char* message, SharedMemory* shared_memory) {
    for (int i = 0; i < SPECIAL_MESS_COUNT; ++i) {
        if (strncmp(message, shared_memory->special_mess[i], MAX_VALUES) == 0) {
            return true;
        }
    }
    return false;
}