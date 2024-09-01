#pragma once
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <random>

#define SHM_NAME "/buffer_shm"
#define SEM_MASTER_MUTEX_NAME "/sem_master_mutex"
#define BUFFER_SIZE 10
#define CONSUMER_COUNT 8
#define BUFFER_COUNT 3

struct SharedBuffer {
    int idx;
    std::vector<std::string> values;
    bool read_by[CONSUMER_COUNT];
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
    std::vector<int> which_buff_cons_have_read[8];
	std::vector<int> fullBuffs;
    std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};
};

int get_random_buffer(int max, const std::vector<int>& excluded);
void clear_times(int buff_num, std::vector<int> (&vecArray)[8]);
void print_buffers(SharedMemory* shared_memory, std::string name, int buffer_index);