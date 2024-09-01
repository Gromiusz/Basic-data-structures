#pragma once
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cstring>

#define SHM_NAME "/buffer_shm"
#define SEM_MASTER_MUTEX_NAME "/sem_master_mutex"
#define BUFFER_SIZE 10
#define CONSUMER_COUNT 8
#define BUFFER_COUNT 3
#define MAX_VALUES 20
#define MAX_FULL_BUFFS 10
#define MAX_CONSUMER_READ_BUFFS 10
#define SPECIAL_MESS_COUNT 8

struct SharedBuffer {
    int idx;
    char values[BUFFER_SIZE][MAX_VALUES];
    bool read_by[BUFFER_SIZE][CONSUMER_COUNT];
    int in;
    int out;
    int count;
    sem_t sem_mutex;
    sem_t sem_empty;
    sem_t sem_full;
    int availableS;
};

struct SharedMemory {
    SharedBuffer buffers[BUFFER_COUNT];
    sem_t sem_master_mutex;
    int which_buff_cons_have_read[CONSUMER_COUNT][MAX_CONSUMER_READ_BUFFS];
    int which_buff_cons_have_read_count[CONSUMER_COUNT];
    int fullBuffs[MAX_FULL_BUFFS];
    int fullBuffs_count;
    char special_mess[SPECIAL_MESS_COUNT][MAX_VALUES];
};

int get_random_buffer(int max, const int* excluded, int excluded_count);
void clear_times(int buff_num, int which_buff_cons_have_read[CONSUMER_COUNT][MAX_CONSUMER_READ_BUFFS], int which_buff_cons_have_read_count[CONSUMER_COUNT]);
void print_buffers(SharedMemory* shared_memory, std::string name, int buffer_index);
bool is_special_message(const char* message, SharedMemory* shared_memory);

