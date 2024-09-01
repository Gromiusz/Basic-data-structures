#include "lib.hpp"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory failed" << std::endl;
        return 1;
    }
    
    ftruncate(shm_fd, sizeof(SharedMemory));
    
    SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Map failed" << std::endl;
        return 1;
    }
    
    if (sem_init(&shared_memory->sem_master_mutex, 1, 1) == -1) {
        std::cerr << "Master semaphore initialization failed" << std::endl;
        return 1;
    }

    if (sem_init(&shared_memory->waiting_producers, 1, 0) == -1) {
        std::cerr << "Master semaphore initialization failed" << std::endl;
        return 1;
    }
    
    for (int i = 0; i < BUFFER_COUNT; ++i) {
        SharedBuffer& buffer = shared_memory->buffers[i];
        buffer.idx = i;
        buffer.in = 0;
        buffer.out = 0;
        buffer.count = 0;
        buffer.availableS = SPECIAL_MESS_COUNT;
        for (int j = 0; j < BUFFER_SIZE; ++j) {
            for (int k = 0; k < CONSUMER_COUNT; ++k) {
                buffer.read_by[j][k] = false;
            }
        }

        if (sem_init(&buffer.sem_mutex, 1, 1) == -1) {
            std::cerr << "Semaphore init failed" << std::endl;
            return 1;
        }
        if (sem_init(&buffer.sem_empty, 1, 0) == -1) {
            std::cerr << "Semaphore init failed" << std::endl;
            return 1;
        }
        if (sem_init(&buffer.sem_full, 1, BUFFER_SIZE) == -1) {
            std::cerr << "Semaphore init failed" << std::endl;
            return 1;
        }
    }
    
    const char* special_messages[SPECIAL_MESS_COUNT] = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};
    for (int i = 0; i < SPECIAL_MESS_COUNT; ++i) {
        strncpy(shared_memory->special_mess[i], special_messages[i], MAX_VALUES - 1);
        shared_memory->special_mess[i][MAX_VALUES - 1] = '\0';
    }
    
    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);
    return 0;
}
