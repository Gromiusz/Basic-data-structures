#include <unistd.h>
#include <random>
#include <cstring>
#include "lib.hpp"
#include <exception>

int probability = 4;


int main(int argc, char *argv[]) {
    if (argc == 2) {
        probability = std::stoi(argv[1]);
        if (probability < 0) {
            std::cerr << "Probability have to be positive" << std::endl;
            return 1;
        }
    }

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory failed: " << strerror(errno) << std::endl;
        return 1;
    }

    SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Map failed: " << strerror(errno) << std::endl;
        close(shm_fd);
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> special_mess_event(0, probability); 
    std::uniform_int_distribution<> special_mess_number(1, 8);
    std::uniform_int_distribution<> normal_mess(65, 90); 

    for (int i = 0; i < 10; ++i) {
        sem_wait(&shared_memory->sem_master_mutex);
        std::string str;
        if (special_mess_event(gen) == 0) {
            int num = special_mess_number(gen);
            str = "S" + std::to_string(num);
        } else {
            char c = static_cast<char>(normal_mess(gen));
            str = std::string(1, c);
        }

        int selectBufferPut = -1;
        int result = -1;
        shared_memory->fullBuffs_count = 0;
        
        for (int j = 0; j < BUFFER_COUNT; ++j) { // preparing the fullBuffs vector
            if (shared_memory->buffers[j].count == BUFFER_SIZE) {
                shared_memory->fullBuffs[shared_memory->fullBuffs_count++] = j;
            }
        }

        selectBufferPut = get_random_buffer(BUFFER_COUNT - 1, shared_memory->fullBuffs, shared_memory->fullBuffs_count);

        if (selectBufferPut == NO_BUFF_SELECTED) {
            shared_memory->waiting_producers_counter++;
            sem_post(&shared_memory->sem_master_mutex);
            sem_wait(&shared_memory->waiting_producers);
            shared_memory->waiting_producers_counter--;
            shared_memory->fullBuffs_count = 0;
            for (int j = 0; j < BUFFER_COUNT; ++j) {
                if (shared_memory->buffers[j].count == BUFFER_SIZE) {
                    shared_memory->fullBuffs[shared_memory->fullBuffs_count++] = j;
                }
            }

            selectBufferPut = get_random_buffer(BUFFER_COUNT - 1, shared_memory->fullBuffs, shared_memory->fullBuffs_count);
            if(selectBufferPut == NO_BUFF_SELECTED) throw std::runtime_error("producer has still no buffer to choice");
        }
        SharedBuffer& buffer = shared_memory->buffers[selectBufferPut];
        
        sem_wait(&buffer.sem_full);
        sem_wait(&buffer.sem_mutex);

        std::string name = "Producer";

        if (is_special_message(str.c_str(), shared_memory)) {
            if (buffer.availableS > 0) {
                buffer.availableS--;
                strncpy(buffer.values[buffer.in], str.c_str(), MAX_VALUES - 1);
                buffer.values[buffer.in][MAX_VALUES - 1] = '\0';
                buffer.in = (buffer.in + 1) % BUFFER_SIZE;
                buffer.count++;
                print_buffers(shared_memory, name, selectBufferPut);
                sem_post(&buffer.sem_empty);
            } else {
                sem_post(&buffer.sem_full);
            }
        } else {
            strncpy(buffer.values[buffer.in], str.c_str(), MAX_VALUES - 1);
            buffer.values[buffer.in][MAX_VALUES - 1] = '\0';
            buffer.in = (buffer.in + 1) % BUFFER_SIZE;
            buffer.count++;
            print_buffers(shared_memory, name, selectBufferPut);
            sem_post(&buffer.sem_empty);
            
        }
        sem_post(&buffer.sem_mutex);

        std::vector<unsigned> buffers_set_to_continue;
        if(buffer.idx == 0)
        {
            buffers_set_to_continue.assign({6, 5, 3, 0}); // a b c ab bc ac abc
        }
        else if(buffer.idx == 1)
        {
            buffers_set_to_continue.assign({6, 4, 3, 1}); // a b c ab bc ac abc
        }
        else if(buffer.idx == 2)
        {
            buffers_set_to_continue.assign({6, 5, 4, 2}); // a b c ab bc ac abc
        }

        while(buffers_set_to_continue.size() > 0)
        {
            int i = buffers_set_to_continue.back();
            buffers_set_to_continue.pop_back();
            if(shared_memory->waiting_consuments_for_buff_set_counter[i] > 0)
            {
                //sem_post(&buffer.sem_mutex);
                sem_post(&shared_memory->waiting_consuments_for_buff_set[i]);
                sem_wait(&shared_memory->sem_master_mutex);
                //sem_wait(&buffer.sem_mutex);
                break;
            }
        }
        sem_post(&shared_memory->sem_master_mutex);

        sleep(1);
    }

    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);
    return 0;
}

