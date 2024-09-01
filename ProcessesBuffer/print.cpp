void print_buffers(SharedMemory* shared_memory, const char* action, int buffer_index) {
    std::cout << "buff 0: [";
    for (int i = 0; i < shared_memory->buffers[0].count; ++i) {
        std::cout << shared_memory->buffers[0].buffer[(shared_memory->buffers[0].out + i) % BUFFER_SIZE];
        if (i < shared_memory->buffers[0].count - 1) std::cout << ", ";
    }
    std::cout << "]";

    if (action != nullptr && buffer_index == 0) {
        std::cout << " <- " << action;
    }

    std::cout << "   ###   buff 1: [";
    for (int i = 0; i < shared_memory->buffers[1].count; ++i) {
        std::cout << shared_memory->buffers[1].buffer[(shared_memory->buffers[1].out + i) % BUFFER_SIZE];
        if (i < shared_memory->buffers[1].count - 1) std::cout << ", ";
    }
    std::cout << "]";

    if (action != nullptr && buffer_index == 1) {
        std::cout << " <- " << action;
    }

    std::cout << "   ###   buff 2: [";
    for (int i = 0; i < shared_memory->buffers[2].count; ++i) {
        std::cout << shared_memory->buffers[2].buffer[(shared_memory->buffers[2].out + i) % BUFFER_SIZE];
        if (i < shared_memory->buffers[2].count - 1) std::cout << ", ";
    }
    std::cout << "]";

    if (action != nullptr && buffer_index == 2) {
        std::cout << " <- " << action;
    }

    std::cout << std::endl;
}



void consume(int consumer_id) {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory failed" << std::endl;
        exit(1);
    }
    
    SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Map failed" << std::endl;
        exit(1);
    }
    
    sem_t *sem_mutex = sem_open(SEM_MUTEX_NAME, 0);
    sem_t *sem_empty = sem_open(SEM_EMPTY_NAME, 0);
    sem_t *sem_full = sem_open(SEM_FULL_NAME, 0);

    if (sem_mutex == SEM_FAILED || sem_empty == SEM_FAILED || sem_full == SEM_FAILED) {
        std::cerr << "Semaphore opening failed" << std::endl;
        exit(1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> buffer_selector(0, BUFFER_COUNT - 1);

    while (true) {
        sem_wait(sem_full);
        sem_wait(sem_mutex);

        int buffer_index = buffer_selector(gen);
        SharedBuffer& buffer = shared_memory->buffers[buffer_index];

        if (buffer.count == 0) {
            sem_post(sem_mutex);
            sem_post(sem_full);
            continue;
        }

        std::string value = buffer.buffer[buffer.out];
        buffer.read_by[buffer.out][consumer_id] = true;

        bool all_read = true;
        for (int i = 0; i < 8; ++i) {
            if (!buffer.read_by[buffer.out][i]) {
                all_read = false;
                break;
            }
        }

        print_buffers(shared_memory, (value[0] == 'S') ? nullptr : (std::string(1, 'A' + consumer_id) + " read ->").c_str(), buffer_index);

        if (all_read || value[0] != 'S') {
            buffer.out = (buffer.out + 1) % BUFFER_SIZE;
            buffer.count--;
            sem_post(sem_empty);
            print_buffers(shared_memory, (value[0] == 'S') ? nullptr : (std::string(1, 'A' + consumer_id) + " rmove ->").c_str(), buffer_index);
        } else {
            sem_post(sem_full);
        }

        sem_post(sem_mutex);

        sleep(1); // Simulate consumption delay
    }

    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <consumer_id>" << std::endl;
        return 1;
    }

    int consumer_id = std::stoi(argv[1]);
    if (consumer_id < 0 || consumer_id >= 8) {
        std::cerr << "Consumer ID must be between 0 and 7" << std::endl;
        return 1;
    }

    consume(consumer_id);
    return 0;
}
