#include <string>
#include <random>
#include "lib.hpp"

std::string consume(int consumer_id) {
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
    
    while (true) {
        sem_wait(&shared_memory->sem_master_mutex);
        int selectedBuffer = -1;
        std::string result = "Err";
        selectedBuffer = get_random_buffer(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[consumer_id]);

        if (selectedBuffer == -2) {
            sem_post(&shared_memory->sem_master_mutex);
            return result;
        }
        if (shared_memory->buffers[selectedBuffer].values.size() == 0) {
            sem_post(&shared_memory->sem_master_mutex);
            return result;
        }
        SharedBuffer& buffer = shared_memory->buffers[selectedBuffer];
        shared_memory->which_buff_cons_have_read[consumer_id].push_back(selectedBuffer);

        int vol1 = buffer.values.size();
        
        sem_wait(&buffer.sem_empty);
        sem_wait(&buffer.sem_mutex);

        result = buffer.values.front();
        buffer.read_by[consumer_id] = true;
        std::string name = "Cons " + std::to_string(consumer_id) + " read";
        print_buffers(shared_memory, name, selectedBuffer);

        bool all_read = true;
        for (int i = 0; i < 8; ++i) {
            if (!buffer.read_by[i]) {
                all_read = false;
                break;
            }
        }

        if (!shared_memory->special_mess.count(result)) {
            buffer.values.erase(buffer.values.begin());
            name = "Cons " + std::to_string(consumer_id) + " rmove";
            print_buffers(shared_memory, name, selectedBuffer);
            buffer.read_by[consumer_id] = false;
            sem_post(&buffer.sem_full);
        } else if (all_read) {
            buffer.values.erase(buffer.values.begin());
            buffer.availableS++;
            name = "Cons " + std::to_string(consumer_id) + " rmove";
            print_buffers(shared_memory, name, selectedBuffer);
            for (int i = 0; i < 8; ++i) {
                buffer.read_by[i] = false;
            }
            sem_post(&buffer.sem_full);
        } else {
            sem_post(&buffer.sem_empty);
        }

        int vol2 = buffer.values.size();
        if (vol2 < vol1) {
            if (!shared_memory->special_mess.count(result))
                shared_memory->which_buff_cons_have_read[consumer_id].erase(std::remove_if(shared_memory->which_buff_cons_have_read[consumer_id].begin(), shared_memory->which_buff_cons_have_read[consumer_id].end(), [selectedBuffer](int num) {return num == selectedBuffer;}), shared_memory->which_buff_cons_have_read[consumer_id].end());
            else
                clear_times(selectedBuffer, shared_memory->which_buff_cons_have_read);
        }

        sem_post(&buffer.sem_mutex);
        sem_post(&shared_memory->sem_master_mutex);

        sleep(1);
        return result;
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

// #include <string>
// #include <random>
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


// std::string consume(int consumer_id) {
//     int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
//     if (shm_fd == -1) {
//         std::cerr << "Shared memory failed" << std::endl;
//         exit(1);
//     }
    
//     SharedMemory* shared_memory = static_cast<SharedMemory*>(mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
//     if (shared_memory == MAP_FAILED) {
//         std::cerr << "Map failed" << std::endl;
//         exit(1);
//     }
    
//     // std::random_device rd;
//     // std::mt19937 gen(rd());
//     // std::uniform_int_distribution<> buffer_selector(0, BUFFER_COUNT - 1);

//     while (true) {
//         sem_wait(&shared_memory->sem_master_mutex);
//         int selectedBuffer=-1;
//         std::string result = "Err";
//         selectedBuffer = get_random_buffer(BUFFER_COUNT-1, shared_memory->which_buff_cons_have_read[consumer_id]);

//         if(selectedBuffer==-2)
// 		{
// 			sem_post(&shared_memory->sem_master_mutex);
// 			return result;
// 		}
// 		if(shared_memory->buffers[selectedBuffer].values.size() == 0)
// 		{
// 			sem_post(&shared_memory->sem_master_mutex);
// 			return result;
// 		}
//         SharedBuffer& buffer = shared_memory->buffers[selectedBuffer];
//         shared_memory->which_buff_cons_have_read[consumer_id].push_back(selectedBuffer);

//         int vol1 = buffer.values.size();
        
//                 sem_wait(&buffer.sem_empty);
//                 sem_wait(&buffer.sem_mutex);

//                 // if (buffer.count == 0) {
//                 //     sem_post(&buffer.sem_mutex);
//                 //     sem_post(&buffer.sem_full);
//                 //     continue;
//                 // }

//                 result = buffer.values.front();
//                 buffer.read_by[consumer_id] = true;
//                 std::string name = "Cons " + std::to_string(consumer_id) + "read";
//                 print_buffers(shared_memory, name, selectedBuffer);

//                 bool all_read = true;
//                 for (int i = 0; i < 8; ++i) {
//                     if (!buffer.read_by[i]) {
//                         all_read = false;
//                         break;
//                     }
//                 }

//                 if(!shared_memory->special_mess.count(result)){
//                     buffer.values.erase( buffer.values.begin());
//                     name = "Cons " + std::to_string(consumer_id) + "rmove";
//                     print_buffers(shared_memory, name, selectedBuffer);
//                     buffer.read_by[consumer_id] = false;
//                     //semA.v();
//                     //full.v();
//                     sem_post(&buffer.sem_full);
// 		        }
//                 else if (all_read){
//                     buffer.values.erase( buffer.values.begin());
//                     buffer.availableS++;
//                     name = "Cons " + std::to_string(consumer_id) + "rmove";
//                     print_buffers(shared_memory, name, selectedBuffer);
//                     for(int i = 0; i < 8; ++i){
//                         buffer.read_by[i] = false;
//                     }
//                     sem_post(&buffer.sem_full);
//                 }
//                 else{
//                     sem_post(&buffer.sem_empty);
//                 }

//                 //print_buffers(shared_memory, (value[0] == 'S') ? nullptr : (std::string(1, 'A' + consumer_id) + " read ->").c_str(), buffer_index);
//                 /// printowanie print("A read");


//                 // bool all_read = true;
//                 // for (int i = 0; i < 8; ++i) {
//                 //     if (!buffer.read_by[buffer.out][i]) {
//                 //         all_read = false;
//                 //         break;
//                 //     }
//                 // }

//                 // print_buffers(shared_memory, (value[0] == 'S') ? nullptr : (std::string(1, 'A' + consumer_id) + " read ->").c_str(), buffer_index);

//                 // if (all_read || value[0] != 'S') {
//                 //     buffer.out = (buffer.out + 1) % BUFFER_SIZE;
//                 //     buffer.count--;
//                 //     sem_post(&buffer.sem_empty);
//                 //     print_buffers(shared_memory, (value[0] == 'S') ? nullptr : (std::string(1, 'A' + consumer_id) + " rmove ->").c_str(), buffer_index);
//                 // } else {
//                 //     sem_post(&buffer.sem_full);
//                 // }


// 		int vol2 = buffer.values.size();
// 		if(vol2<vol1)
// 			if(!shared_memory->special_mess.count(result))
// 				shared_memory->which_buff_cons_have_read[consumer_id].erase(std::remove_if(shared_memory->which_buff_cons_have_read[consumer_id].begin(), shared_memory->which_buff_cons_have_read[consumer_id].end(), [selectedBuffer](int num) {return num == selectedBuffer;}), shared_memory->which_buff_cons_have_read[consumer_id].end());
// 			else
// 				clear_times(selectedBuffer, shared_memory->which_buff_cons_have_read);

//         sem_post(&buffer.sem_mutex);
//         sem_post(&shared_memory->sem_master_mutex);

//         sleep(1);
//         return result;
//     }

//     munmap(shared_memory, sizeof(SharedMemory));
//     close(shm_fd);
// }

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <consumer_id>" << std::endl;
//         return 1;
//     }

//     int consumer_id = std::stoi(argv[1]);
//     if (consumer_id < 0 || consumer_id >= 8) {
//         std::cerr << "Consumer ID must be between 0 and 7" << std::endl;
//         return 1;
//     }

//     consume(consumer_id);
//     return 0;
// }
