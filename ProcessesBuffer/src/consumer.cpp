#include <string>
#include <random>
#include "lib.hpp"

// const std::vector<int> vec_0{0};
// const std::vector<int> vec_1{1};
// const std::vector<int> vec_2{2};
// const std::vector<int> vec_01{0,1};
// const std::vector<int> vec_12{1,2};
// const std::vector<int> vec_03{0,3};
// const std::vector<int> vec_012{0,1,2};

std::vector<int> fixed_vec[7];
 

std::string consume(int consumer_id, SharedMemory* shared_memory) {
    while (true) {
        sleep(1);
        sem_wait(&shared_memory->sem_master_mutex);
        int selectedBuffer = -1;
        std::string result = "Err";
        selectedBuffer = get_random_buffer(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id], shared_memory->which_buff_cons_have_read_count[consumer_id]);



        if (selectedBuffer == NO_BUFF_SELECTED) {
            shared_memory->waiting_consuments_for_any_buff_counter++;
            sem_post(&shared_memory->sem_master_mutex);
            sem_wait(&shared_memory->waiting_consuments_for_any_buff);
            shared_memory->waiting_consuments_for_any_buff_counter--;
            selectedBuffer = get_random_buffer(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id], shared_memory->which_buff_cons_have_read_count[consumer_id]);
            if(selectedBuffer == NO_BUFF_SELECTED) throw std::runtime_error("consumer has still no buffer to choice");
            // return result;
        }
        std::vector<int> possible_buffers = get_possible_buffers(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id], shared_memory->which_buff_cons_have_read_count[consumer_id]);

        bool condition0 = (shared_memory->buffers[0].count == 0);
        bool condition1 = (shared_memory->buffers[1].count == 0);
        bool condition2 = (shared_memory->buffers[2].count == 0);

        bool all_conditions[7] = {
            condition0, // shared_memory->buffers[0].count == 0
            condition1, // shared_memory->buffers[1].count == 0
            condition2, // shared_memory->buffers[2].count == 0
            condition0 && condition1, // shared_memory->buffers[0].count == 0 && shared_memory->buffers[1].count == 0
            condition1 && condition2, // shared_memory->buffers[1].count == 0 && shared_memory->buffers[2].count == 0
            condition0 && condition2, // shared_memory->buffers[0].count == 0 && shared_memory->buffers[2].count == 0
            condition0 && condition1 && condition2 // shared_memory->buffers[0].count == 0 && shared_memory->buffers[1].count == 0 && shared_memory->buffers[2].count == 0
        };

        for(int i = 0; i < 7; i++)
        {
            if(possible_buffers == fixed_vec[i] && all_conditions[i]) {
                shared_memory->waiting_consuments_for_buff_set_counter[i]++;
                sem_post(&shared_memory->sem_master_mutex);
                sem_wait(&shared_memory->waiting_consuments_for_buff_set[i]);
                shared_memory->waiting_consuments_for_buff_set_counter[i]--;
                break;
            }
        }

        if (shared_memory->buffers[selectedBuffer].count == 0) {
            //std::vector<int> possible_buffers = get_possible_buffers(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id], shared_memory->which_buff_cons_have_read_count[consumer_id]);
            possible_buffers.erase(std::remove(possible_buffers.begin(), possible_buffers.end(), selectedBuffer), possible_buffers.end());
            while (possible_buffers.size() > 0)
            {
                selectedBuffer = possible_buffers.back();
                possible_buffers.pop_back();
                if(shared_memory->buffers[selectedBuffer].count != 0) break;
            }
        }

        // if (shared_memory->buffers[selectedBuffer].count == 0) {
        //     shared_memory->waiting_consuments_for_concrete_buff_counter[selectedBuffer]++;
        //     sem_post(&shared_memory->sem_master_mutex);
        //     sem_wait(&shared_memory->waiting_consuments_for_concrete_buff[selectedBuffer]);
        //     shared_memory->waiting_consuments_for_concrete_buff_counter[selectedBuffer]--;
        //     return result;
        // }
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
            
            if(shared_memory->waiting_producers_counter > 0)
            {
                sem_post(&shared_memory->waiting_producers);
                sem_wait(&shared_memory->sem_master_mutex);
            }
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

    fixed_vec[0].assign({0});
    fixed_vec[1].assign({1});
    fixed_vec[2].assign({2});
    fixed_vec[3].assign({0, 1});
    fixed_vec[4].assign({1, 2});
    fixed_vec[5].assign({0, 2});
    fixed_vec[6].assign({1, 2, 3});

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
