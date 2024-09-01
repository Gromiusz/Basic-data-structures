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
    
    // sem_t *sem_master_mutex = sem_open(SEM_MASTER_MUTEX_NAME, O_CREAT, 0666, 1);

    if (sem_init(&shared_memory->sem_master_mutex, 1, 1) == -1) {
        std::cerr << "Master semaphore initialization failed" << std::endl;
        return 1;
    }
    
    for (int i = 0; i < BUFFER_COUNT; ++i) {
        SharedBuffer& buffer = shared_memory->buffers[i];
        buffer.in = 0;
        buffer.out = 0;
        buffer.count = 0;
        buffer.availableS = 8;
        // for (int j = 0; j < BUFFER_SIZE; ++j) {
            for (int k = 0; k < CONSUMER_COUNT; ++k) {
                buffer.read_by[k] = false;
            }
        //}

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
    
    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);
    return 0;
}
