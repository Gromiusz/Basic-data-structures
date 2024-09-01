#include <unistd.h>
#include <random>
#include <cstring>
#include "lib.hpp"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory failed" << std::endl;
        return 1;
    }
    
    SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Map failed" << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> special_mess_event(0, 4); // Assuming probability = 4
    std::uniform_int_distribution<> special_mess_number(1, 8);
    std::uniform_int_distribution<> normal_mess(65, 90); // A-Z ASCII

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
        shared_memory->fullBuffs.clear();
		
        for(int j = 0; j < BUFFER_COUNT; ++j){
            if(shared_memory->buffers[j].values.size() == BUFFER_SIZE){
                shared_memory->fullBuffs.push_back(j);
            }
        }
        selectBufferPut = get_random_buffer(BUFFER_COUNT - 1, shared_memory->fullBuffs);

        SharedBuffer& buffer = shared_memory->buffers[0];
        if(selectBufferPut != -2) {
            buffer = shared_memory->buffers[selectBufferPut];
            result = 0;
        } else {
            sem_post(&shared_memory->sem_master_mutex);
            --i; // Decrement i to retry the current iteration
            continue;
        }
        
        sem_wait(&buffer.sem_full);
        sem_wait(&buffer.sem_mutex);

        std::string name = "Producer";

        if(shared_memory->special_mess.count(str)) {
            if(buffer.availableS > 0) {
                buffer.availableS--;
                buffer.values.push_back(str);
                print_buffers(shared_memory, name, selectBufferPut);
                sem_post(&buffer.sem_empty);
                sem_post(&buffer.sem_mutex);
            } else {
                sem_post(&buffer.sem_full);
                sem_post(&buffer.sem_mutex);
            }
        } else {
            buffer.values.push_back(str);
            print_buffers(shared_memory, name, selectBufferPut);
            sem_post(&buffer.sem_empty);
            sem_post(&buffer.sem_mutex);
        }

        sem_post(&shared_memory->sem_master_mutex);

        sleep(1); // Simulate production delay
    }

    munmap(shared_memory, sizeof(SharedMemory));
    close(shm_fd);
    return 0;
}

// #include <unistd.h>
// #include <random>
// #include <cstring>
// #include "lib.hpp"

// // void print_buffers(SharedMemory* shared_memory, const char* action, int buffer_index) {
// //     std::cout << "buff 0: [";
// //     for (int i = 0; i < shared_memory->buffers[0].count; ++i) {
// //         std::cout << shared_memory->buffers[0].buffer[(shared_memory->buffers[0].out + i) % BUFFER_SIZE];
// //         if (i < shared_memory->buffers[0].count - 1) std::cout << ", ";
// //     }
// //     std::cout << "]";

// //     std::cout << "   ###   buff 1: [";
// //     for (int i = 0; i < shared_memory->buffers[1].count; ++i) {
// //         std::cout << shared_memory->buffers[1].buffer[(shared_memory->buffers[1].out + i) % BUFFER_SIZE];
// //         if (i < shared_memory->buffers[1].count - 1) std::cout << ", ";
// //     }
// //     std::cout << "]";

// //     std::cout << "   ###   buff 2: [";
// //     for (int i = 0; i < shared_memory->buffers[2].count; ++i) {
// //         std::cout << shared_memory->buffers[2].buffer[(shared_memory->buffers[2].out + i) % BUFFER_SIZE];
// //         if (i < shared_memory->buffers[2].count - 1) std::cout << ", ";
// //     }
// //     std::cout << "]";

// //     if (action != nullptr) {
// //         std::cout << " <- " << action << " buffer " << buffer_index;
// //     }

// //     std::cout << std::endl;
// // }

// int main() {
//     int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
//     if (shm_fd == -1) {
//         std::cerr << "Shared memory failed" << std::endl;
//         return 1;
//     }
    
//     SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
//     if (shared_memory == MAP_FAILED) {
//         std::cerr << "Map failed" << std::endl;
//         return 1;
//     }

//     // sem_t *sem_master_mutex = sem_open(SEM_MUTEX_NAME, 0);
//     // if (sem_master_mutex == SEM_FAILED) {
//     //     std::cerr << "Semaphore opening failed" << std::endl;
//     //     exit(1);
//     // }

    
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> special_mess_event(0, 4); // Assuming probability = 4
//     std::uniform_int_distribution<> special_mess_number(1, 8);
//     std::uniform_int_distribution<> normal_mess(65, 90); // A-Z ASCII
//     //std::uniform_int_distribution<> buffer_selector(0, BUFFER_COUNT - 1);

//     for (int i = 0; i < 10; ++i) {
//         sem_wait(&shared_memory->sem_master_mutex);
//         std::string str;
//         if (special_mess_event(gen) == 0) {
//             int num = special_mess_number(gen);
//             str = "S" + std::to_string(num);
//         } else {
//             char c = static_cast<char>(normal_mess(gen));
//             str = std::string(1, c);
//         }

//         int selectBufferPut = -1;
//         int result = -1;
//         shared_memory->fullBuffs.clear();
		
//         for(int i=0; i<BUFFER_COUNT; ++i){
//             if(shared_memory->buffers[i].values.size() == BUFFER_SIZE){
//                 shared_memory->fullBuffs.push_back(i);
//             }
//         }
// 		selectBufferPut = get_random_buffer(BUFFER_COUNT-1, shared_memory->fullBuffs);

//         SharedBuffer& buffer = shared_memory->buffers[0];
//         if(selectBufferPut!=-2){
// 			buffer = shared_memory->buffers[selectBufferPut];
// 			result = 0;
// 		}
//         else{
//             sem_post(&shared_memory->sem_master_mutex);
//             i--;
//             continue;
//         }
        

//         sem_wait(&buffer.sem_full);
//         sem_wait(&buffer.sem_mutex);

//         std::string name = "Producer";

//         if(shared_memory->special_mess.count(str)) {
// 			if(buffer.availableS>0){
// 				buffer.availableS--;
// 				buffer.values.push_back(str);
				
//                 print_buffers(shared_memory, name, selectBufferPut);
//                 sem_post(&buffer.sem_empty);
//                 sem_post(&buffer.sem_mutex);
// 			}
// 			else{
//                 sem_post(&buffer.sem_full);
//                 sem_post(&buffer.sem_mutex);
// 			}
// 		}
// 		else
// 		{
// 			buffer.values.push_back(str);
// 			print_buffers(shared_memory, name, selectBufferPut);
// 			sem_post(&buffer.sem_empty);
//             sem_post(&buffer.sem_mutex);

// 		}

//         // strcpy(buffer.buffer[buffer.in], str.c_str());
//         // for (int j = 0; j < 8; ++j) {
//         //     buffer.read_by[buffer.in][j] = false;
//         // }
//         // buffer.in = (buffer.in + 1) % BUFFER_SIZE;
//         // buffer.count++;

//         // print_buffers(shared_memory, "Produced", buffer_index);

//         // sem_post(&buffer.sem_mutex);
//         // sem_post(&buffer.sem_full);
//         sem_post(&shared_memory->sem_master_mutex);

//         sleep(1); // Simulate production delay
//     }

//     munmap(shared_memory, sizeof(SharedMemory));
//     close(shm_fd);
//     return 0;
// }
