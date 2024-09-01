#include <string>
#include <random>
#include "lib.hpp"

std::string consume(int consumer_id, SharedMemory* shared_memory) {
    while (true) {
        sleep(1);
        sem_wait(&shared_memory->sem_master_mutex);
        int selectedBuffer = -1;
        std::string result = "Err";
        selectedBuffer = get_random_buffer(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id], shared_memory->which_buff_cons_have_read_count[consumer_id]);

        if (selectedBuffer == -2) {
            sem_post(&shared_memory->sem_master_mutex);
            return result;
        }
        if (shared_memory->buffers[selectedBuffer].count == 0) {
            sem_post(&shared_memory->sem_master_mutex);
            return result;
        }
        SharedBuffer& buffer = shared_memory->buffers[selectedBuffer];
        shared_memory->which_buff_cons_have_read[consumer_id][shared_memory->which_buff_cons_have_read_count[consumer_id]++] = selectedBuffer;

        int vol1 = buffer.count;
        
        sem_wait(&buffer.sem_empty);
        sem_wait(&buffer.sem_mutex);

        result = buffer.values[buffer.out];
        buffer.read_by[buffer.out][consumer_id] = true;
        std::string name = "Cons " + std::to_string(consumer_id) + " read";
        print_buffers(shared_memory, name, selectedBuffer);

        bool all_read = true;
        for (int i = 0; i < CONSUMER_COUNT; ++i) {
            if (!buffer.read_by[buffer.out][i]) {
                all_read = false;
                break;
            }
        }

        if (!is_special_message(result.c_str(), shared_memory)) {
            name = "Cons " + std::to_string(consumer_id) + " rmove";
            print_buffers(shared_memory, name, selectedBuffer);
            buffer.read_by[buffer.out][consumer_id] = false;
            buffer.out = (buffer.out + 1) % BUFFER_SIZE;  // Move out index forward
            buffer.count--;
            sem_post(&buffer.sem_full);
        } else if (all_read) {
            name = "Cons " + std::to_string(consumer_id) + " rmove";
            print_buffers(shared_memory, name, selectedBuffer);
            for (int k = 0; k < CONSUMER_COUNT; ++k) {
                buffer.read_by[buffer.out][k] = false;
            }
            buffer.out = (buffer.out + 1) % BUFFER_SIZE;  // Move out index forward
            buffer.count--;
            buffer.availableS++;
            sem_post(&buffer.sem_full);
        } else {
            sem_post(&buffer.sem_empty);
        }

        int vol2 = buffer.count;
        if (vol2 < vol1) {
            if (!is_special_message(result.c_str(), shared_memory))
                shared_memory->which_buff_cons_have_read_count[consumer_id]--;
            else
                clear_times(selectedBuffer, shared_memory->which_buff_cons_have_read, shared_memory->which_buff_cons_have_read_count);
        }

        sem_post(&buffer.sem_mutex);
        sem_post(&shared_memory->sem_master_mutex);

        sleep(1);
        return result;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <consumer_id>" << std::endl;
        return 1;
    }

    int consumer_id = std::stoi(argv[1]);
    if (consumer_id < 0 || consumer_id >= CONSUMER_COUNT) {
        std::cerr << "Consumer ID must be between 0 and " << CONSUMER_COUNT - 1 << std::endl;
        return 1;
    }

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

    while (true){
        bool all_empty = true;
        sem_wait(&shared_memory->sem_master_mutex);
        for(int i = 0; i < BUFFER_COUNT; i++) {
            if(shared_memory->buffers[i].count != 0) {
                all_empty = false;
                break;
            }
        }
        sem_post(&shared_memory->sem_master_mutex);
        if (all_empty) {
            munmap(shared_memory, sizeof(SharedMemory));
            close(shm_fd);
            break;
        }
        consume(consumer_id, shared_memory);
    }
    std::cout << "Consumer "<< consumer_id << " Done" << std::endl;
    return 0;
}
