#include "lib.hpp"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory not found" << std::endl;
        return 1;
    }

    SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Map failed" << std::endl;
        return 1;
    }

    // Clean up semaphores
    for (int i = 0; i < BUFFER_COUNT; ++i) {
        sem_destroy(&shared_memory->buffers[i].sem_mutex);
        sem_destroy(&shared_memory->buffers[i].sem_empty);
        sem_destroy(&shared_memory->buffers[i].sem_full);
    }

    sem_destroy(&shared_memory->sem_master_mutex);

    // Unmap shared memory
    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);

    // Remove shared memory
    shm_unlink(SHM_NAME);

    std::cout << "Cleanup completed." << std::endl;

    return 0;
}

