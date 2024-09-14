#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"
#include <random>
#include <set>
#include <algorithm>
#include <chrono> 
#include <iomanip>
#include <thread>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NO_BUFF_SELECTED -2

constexpr unsigned CONSUMERS_COUNT = 8;
constexpr unsigned BUFFERS_COUNT = 3;
constexpr unsigned BUFFER_SIZE = 10;
constexpr unsigned THREADS_COUNT = 13;  // number of threads 13
constexpr unsigned ALL_BUFFERS_SET = 7; // a b c ab ac abc

int probability;
int delay;

class Buffer
{
private:
	Semaphore mutex;
	Semaphore empty;
	Semaphore full;
	Semaphore sem[8]; // A B C ... H
	Semaphore semS;
	int available_S;
	int counter;
	bool read_[8]; // A B C ... H
	bool do_not_print[8];
	// std::vector<bool*> readVec;
	static Semaphore printSem;
	
	// std::vector<std::string> values;
	std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};

	void print(std::string name)
	{
		printSem.p();
		which_buffer_read();
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		std::string nameP = "";
		std::string nameC = "";
		if(name=="Producer"){
			nameP = "<- " + name;
		} else {
			nameC = name + " ->";
		}
		std::cout << "\n Buff: " << buffIndex<<  " [size:" << values.size() << "]    ### " << nameC << " [";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] " << nameP << " ###    Who has read: ";
		who_read();
		std::cout << "\n" << ANSI_COLOR_RESET;
		printSem.v();
	}

	void which_buffer_read(){
		switch (buffIndex)
		{
			case 0: std::cout << ANSI_COLOR_RED; break;
			case 1: std::cout << ANSI_COLOR_GREEN; break;
			case 2: std::cout << ANSI_COLOR_MAGENTA; break;
			default: break;
		}
	}

	void who_read() {
		char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
		bool read[] = {read_[0], read_[1], read_[2], read_[3], read_[4], read_[5], read_[6], read_[7]};

		for (int i = 0; i < sizeof(letters) / sizeof(letters[0]); ++i) {
			std::cout << (read[i] ? letters[i] : '_');
		}
	}

public:
	std::vector<std::string> values;
	int buffIndex;
	Buffer() : mutex(1), empty(0), full(BUFFER_SIZE), sem{Semaphore(1), Semaphore(1),Semaphore(1),Semaphore(1),Semaphore(1),Semaphore(1),Semaphore(1),Semaphore(1)}, semS(CONSUMERS_COUNT), available_S(CONSUMERS_COUNT), counter(0)
	{
		for(int i=0; i<CONSUMERS_COUNT; i++)
		{
			read_[i] = false;
			do_not_print[i]=true;
		}
	}
	int get_buff_size()
	{
		return values.size();
	}
	bool is_empty()
	{
		return counter == 0;
	}

	bool is_full()
	{
		return values.size()==BUFFER_SIZE;
	}

	void put(std::string value)
	{
		full.p();
		mutex.p();
		if(special_mess.count(value)) {
			if(available_S>0){
				semS.p();
				available_S--;
				values.push_back(value);
				print("Producer");
				empty.v();
				counter++;
			}
			else{
				full.v();
			}
		}
		else
		{
			values.push_back(value);
			print("Producer");
			empty.v();
			counter++;
		}
		mutex.v();
	}

	std::string get(unsigned who)
	{
		sem[who].p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		read_[who] = true;
		char ch = static_cast<char>(65 + who);
		std::string whoStr (1, ch);
		print(whoStr + " read");

		bool all_read_except_who = true;
		for(unsigned i=0;i<CONSUMERS_COUNT;i++)
		{
			if(i != who)
			{
				all_read_except_who = all_read_except_who && read_[i];
			}
		}

		if(!special_mess.count(v)){
			values.erase(values.begin());
			print(whoStr + " remove");
			read_[who] = false;
			counter--;
			sem[who].v();
			full.v();
		}
		else if (all_read_except_who){
			// remove element from buffer
			available_S++;
			semS.v();
			values.erase(values.begin());
			print(whoStr + " remove");
			for(unsigned i=0;i<CONSUMERS_COUNT;i++)
			{
				read_[i] = false;
				sem[i].v();
			}
			counter--;
			full.v();
		}
		else{
			empty.v();
		}
		mutex.v();
		return v;
	}

};

class BufferCollection{
	std::vector<Buffer*> bufferCollection;
	std::vector<int> get_possible_buffers(int max, const int* excluded, int excluded_count)
	{
		std::vector<int> available_numbers;
		for (int i = 0; i <= max; ++i)
		{
			if (std::find(excluded, excluded + excluded_count, i) == excluded + excluded_count)
			{
				available_numbers.push_back(i);
			}
		}

		return available_numbers;
	}

public:
	
	std::vector<int> times[CONSUMERS_COUNT];
	Semaphore masterMutex;
	Semaphore waiting_producers;
	Semaphore waiting_consuments_for_any_buff;
    Semaphore waiting_consuments_for_buff_set[ALL_BUFFERS_SET];
	std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};

	int buffIndexes;

	int visit_left;

	unsigned waiting_consuments_for_buff_set_counter[ALL_BUFFERS_SET];
    unsigned waiting_producers_counter = 0;
    unsigned waiting_consuments_for_any_buff_counter = 0;
    int which_buff_cons_have_read[CONSUMERS_COUNT][BUFFER_SIZE];
    int which_buff_cons_have_read_count[CONSUMERS_COUNT];
    int fullBuffs[BUFFER_SIZE]; // a structure that contains indexes of full buffers
    int fullBuffs_count = 0; // a lenght of above "vector"
    //char special_mess[SPECIAL_MESS_COUNT][MAX_VALUES];

	BufferCollection(): visit_left(0), masterMutex(1), waiting_producers(0), waiting_consuments_for_any_buff(0), buffIndexes(0), waiting_consuments_for_buff_set{Semaphore(0), Semaphore(0), Semaphore(0), Semaphore(0), Semaphore(0), Semaphore(0), Semaphore(0)} 
	{
		for(int i=0; i<ALL_BUFFERS_SET; i++)
		{
			waiting_consuments_for_buff_set_counter[i] = 0;
		}
		for(int i=0; i<CONSUMERS_COUNT; i++)
		{
			which_buff_cons_have_read_count[i] = 0;
		}
	}

	void add_buffer(Buffer * buffer)
	{
		bufferCollection.push_back(buffer);
		bufferCollection[buffIndexes]->buffIndex = buffIndexes;
		std::cout << "Buffer added to collection\n";
		buffIndexes++;
	}

	bool areAllBuffersEmpty()
	{
		bool odp=true;
		for(const auto &buff: bufferCollection)
		{
			odp = odp && buff->is_empty();
		}
		return odp;
	}

	// void clear_times(int buff_num)
	// {
	// 	for(unsigned i=0; i<CONSUMERS_COUNT;i++)
	// 	{
	// 		times[i].erase(std::remove_if(times[i].begin(), times[i].end(), [buff_num](int num) {
    //     		return num == buff_num;}), times[i].end());
	// 	}
	// }
	void clear_times(int buff_num, int which_buff_cons_have_read[CONSUMERS_COUNT][BUFFER_SIZE], int which_buff_cons_have_read_count[CONSUMERS_COUNT])
	{
		for (int i = 0; i < CONSUMERS_COUNT; ++i) {
			int new_count = 0;
			for (int j = 0; j < which_buff_cons_have_read_count[i]; ++j) {
				if (which_buff_cons_have_read[i][j] != buff_num) {
					which_buff_cons_have_read[i][new_count++] = which_buff_cons_have_read[i][j];
				}
			}
			which_buff_cons_have_read_count[i] = new_count;
		}
	}

	int put(std::string value) // was void
	{
		masterMutex.p();
		int selectBufferPut = -1;
		int tryCounter = 0;
		int result = -1;

		fullBuffs_count = 0;
		for (int j = 0; j < BUFFERS_COUNT; ++j) { // preparing the fullBuffs vector
            if (bufferCollection[j]->is_full()) {
                fullBuffs[fullBuffs_count++] = j;
            }
        }

        selectBufferPut = get_random_buffer(BUFFERS_COUNT - 1, fullBuffs, fullBuffs_count);

        if (selectBufferPut == NO_BUFF_SELECTED) {
            waiting_producers_counter++;
			masterMutex.v();
			waiting_producers.p();
            waiting_producers_counter--;

            fullBuffs_count = 0;
            for (int j = 0; j < BUFFERS_COUNT; ++j) {
                if (bufferCollection[j]->is_full()) {
                    fullBuffs[fullBuffs_count++] = j;
                }
            }

            selectBufferPut = get_random_buffer(BUFFERS_COUNT - 1, fullBuffs, fullBuffs_count);
            if(selectBufferPut == NO_BUFF_SELECTED) throw std::runtime_error("producer has still no buffer to choice");
        }
		bufferCollection[selectBufferPut]->put(value);

		std::vector<unsigned> buffers_set_to_continue;
        if(selectBufferPut == 0)
        {
            buffers_set_to_continue.assign({6, 5, 3, 0}); // a b c ab bc ac abc
        }
        else if(selectBufferPut == 1)
        {
            buffers_set_to_continue.assign({6, 4, 3, 1}); // a b c ab bc ac abc
        }
        else if(selectBufferPut == 2)
        {
            buffers_set_to_continue.assign({6, 5, 4, 2}); // a b c ab bc ac abc
        }

        while(buffers_set_to_continue.size() > 0)
        {
            int i = buffers_set_to_continue.back();
            buffers_set_to_continue.pop_back();
            if(waiting_consuments_for_buff_set_counter[i] > 0)
            {
            	waiting_consuments_for_buff_set[i].v();
                masterMutex.p();
                break;
            }
        }
		// do{
		// 	selectBufferPut = get_random_buffer(bufferCollection.size()-1);
		// 	tryCounter++;
		// }while(bufferCollection[selectBufferPut]->is_full() && tryCounter<10);
		// if(tryCounter<10){
		// 	bufferCollection[selectBufferPut]->put(value);
		// 	result = 0;
		// }
		masterMutex.v();
		return result;
	}

	template<typename Getter, typename TimesVec>
	std::string get(Getter getter, TimesVec& timesVec, const unsigned& who)
	{
		masterMutex.p();
		int selectedBuffer=-1;
		// int tries = 0;
		// do{
		// 	do{
		// 		selectedBuffer = get_random_buffer(bufferCollection.size()-1);
		// 		tries++;
		// 		if(tries>20){
		// 			std::string result = "Err";
		// 			masterMutex.v();
		// 			return result;
		// 		}
		// 	}while (std::find(timesVec.begin(), timesVec.end(), selectedBuffer) != timesVec.end());
		// 	tries++;
		// 	if(tries>20){
		// 		std::string result = "Err";
		// 		masterMutex.v();
		// 		return result;
		// 	}

		// }while(bufferCollection[selectedBuffer]->is_empty());
		// timesVec.push_back(selectedBuffer);
		std::string result = "Err";
        selectedBuffer = get_random_buffer(BUFFERS_COUNT - 1, which_buff_cons_have_read[who], which_buff_cons_have_read_count[who]);

        if (selectedBuffer == NO_BUFF_SELECTED) {
            waiting_consuments_for_any_buff_counter++;
            masterMutex.v();
            waiting_consuments_for_any_buff.p();
            waiting_consuments_for_any_buff_counter--;
            selectedBuffer = get_random_buffer(BUFFERS_COUNT - 1, which_buff_cons_have_read[who], which_buff_cons_have_read_count[who]);
            if(selectedBuffer == NO_BUFF_SELECTED) throw std::runtime_error("consumer has still no buffer to choice");
            // return result;
        }
        std::vector<int> possible_buffers = get_possible_buffers(BUFFERS_COUNT - 1, which_buff_cons_have_read[who], which_buff_cons_have_read_count[who]);

        bool condition0 = bufferCollection[0]->is_empty();
        bool condition1 = bufferCollection[1]->is_empty();
        bool condition2 = bufferCollection[2]->is_empty();


        bool all_conditions[7] = {
            condition0, 
            condition1, // shared_memory->buffers[1].count == 0
            condition2, // shared_memory->buffers[2].count == 0
            condition0 && condition1, // shared_memory->buffers[0].count == 0 && shared_memory->buffers[1].count == 0
            condition1 && condition2, // shared_memory->buffers[1].count == 0 && shared_memory->buffers[2].count == 0
            condition0 && condition2, // shared_memory->buffers[0].count == 0 && shared_memory->buffers[2].count == 0
            condition0 && condition1 && condition2 // shared_memory->buffers[0].count == 0 && shared_memory->buffers[1].count == 0 && shared_memory->buffers[2].count == 0
        };

        const std::vector<int> vec_0{0};
        const std::vector<int> vec_1{1};
        const std::vector<int> vec_2{2};
        const std::vector<int> vec_01{0,1};
        const std::vector<int> vec_12{1,2};
		const std::vector<int> vec_02{0,2};
        const std::vector<int> vec_012{0,1,2};

        std::vector<std::vector<int>> contener = {vec_0, vec_1, vec_2, vec_01, vec_12, vec_02, vec_012};
        auto found_it = std::find(contener.begin(), contener.end(), possible_buffers);

        unsigned j = std::distance(contener.begin(),found_it);

        // POTRZEBUJE CZEGOŚ W RODZAJU:
        // IF possible_buffers == vec_0: j = 0
        // if possible_buffers == vec_1: j = 1


        // for(int i = 0; i < 7; i++)
        // {
            if(all_conditions[j]) {
                waiting_consuments_for_buff_set_counter[j]++;
                masterMutex.v();
                waiting_consuments_for_buff_set[j].p();
                waiting_consuments_for_buff_set_counter[j]--;
                //break;
            }
        //}

        selectedBuffer = get_random_buffer(BUFFERS_COUNT - 1, which_buff_cons_have_read[who], which_buff_cons_have_read_count[who]);

        if (bufferCollection[selectedBuffer]->is_empty()) {
            //std::vector<int> possible_buffers = get_possible_buffers(BUFFER_COUNT - 1, shared_memory->which_buff_cons_have_read[who], shared_memory->which_buff_cons_have_read_count[who]);
            possible_buffers.erase(std::remove(possible_buffers.begin(), possible_buffers.end(), selectedBuffer), possible_buffers.end());
            while (possible_buffers.size() > 0)
            {
                selectedBuffer = possible_buffers.back();
                possible_buffers.pop_back();
                if(!bufferCollection[selectedBuffer]->is_empty()) break;
            }
        }
		which_buff_cons_have_read[who][which_buff_cons_have_read_count[who]++] = selectedBuffer;
		int vol1 = bufferCollection[selectedBuffer]->get_buff_size();
		result = (bufferCollection[selectedBuffer]->*getter)(who);
		int vol2 = bufferCollection[selectedBuffer]->get_buff_size();
		if(vol2<vol1)
		{
			bool one = false;
			if (!special_mess.count(result))
                which_buff_cons_have_read_count[who]--;
            else
            {
				clear_times(selectedBuffer, which_buff_cons_have_read, which_buff_cons_have_read_count);
                //clear_times(selectedBuffer);
                if(waiting_consuments_for_any_buff_counter > 0)
                {
					waiting_consuments_for_any_buff.v();
					masterMutex.p();
					one = true;
                }
            }
			if(waiting_producers_counter > 0 && !one)
            {
				waiting_producers.v();
				masterMutex.p();
            }
			// which_buff_cons_have_read_count[who]--;
			// if (special_mess.count(result))  
			// {
			// 	clear_times(selectedBuffer, which_buff_cons_have_read, which_buff_cons_have_read_count);
            //     //clear_times(selectedBuffer);
            //     if(waiting_consuments_for_any_buff_counter > 0)
            //     {
			// 		waiting_consuments_for_any_buff.v();
			// 		masterMutex.p();
            //     }
            // }
			// else if(waiting_producers_counter > 0)
            // {
			// 	waiting_producers.v();
			// 	masterMutex.p();
            // }
		}
			
		masterMutex.v();
		return result;
	}

	int get_random_buffer(int max, const int* excluded, int excluded_count)
	{
		// std::random_device rd;
    	// std::mt19937 gen(rd());
    	// std::uniform_int_distribution<> selectBuffer(0, max);
		// return selectBuffer(gen);
		std::vector<int> available_numbers;
		for (int i = 0; i <= max; ++i)
		{
			if (std::find(excluded, excluded + excluded_count, i) == excluded + excluded_count)
			{
				available_numbers.push_back(i);
			}
		}
		if (available_numbers.empty())
		{
			return -2;
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, available_numbers.size() - 1);

		return available_numbers[dist(gen)];
	}
};

BufferCollection buffer;
Buffer buff1;
Buffer buff2;
Buffer buff3;
Semaphore Buffer::printSem(1);

int lastMess = 0;
Semaphore lastMessTh(1);

void* threadProd(void* arg)
{
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> special_mess_event(0, probability);
	std::uniform_int_distribution<> special_mess_number(1, 8);
	std::string str;
	for (int i = 0; i < 10; ++i)
	{
		if(special_mess_event(gen)==0){
			int num = special_mess_number(gen);
			str = "S" + std::to_string(num);
		}
		else {
			int j = i%26 + 65;
			str = std::string(1, static_cast<char>(j));
		}
		//buffer.put(str);
		if(buffer.put(str)==-1){
			i--;
		}
	}
	lastMessTh.p();
	lastMess++;
	lastMessTh.v();
	
	return NULL;
}


struct ThreadArgs
{
	std::string (Buffer::*getter)(unsigned);
	std::vector<int>& times;
	unsigned who;
};


void* threadCons(void* arg)
{
	ThreadArgs* tArgs = static_cast<ThreadArgs*>(arg);
	while (true)
	{
		auto value = buffer.get(tArgs->getter, tArgs->times, tArgs->who);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

ThreadArgs tArgA = {&Buffer::get, buffer.times[0], 0};
ThreadArgs tArgB = {&Buffer::get, buffer.times[1], 1};
ThreadArgs tArgC = {&Buffer::get, buffer.times[2], 2};
ThreadArgs tArgD = {&Buffer::get, buffer.times[3], 3};
ThreadArgs tArgE = {&Buffer::get, buffer.times[4], 4};
ThreadArgs tArgF = {&Buffer::get, buffer.times[5], 5};
ThreadArgs tArgG = {&Buffer::get, buffer.times[6], 6};
ThreadArgs tArgH = {&Buffer::get, buffer.times[7], 7};


int main(int argc, char *argv[]) {
    if (argc >= 3) {
        probability = std::atoi(argv[1]);
		delay = std::atoi(argv[2]);
    } else {
        probability = 4;
		delay = 1;
    }
	
#ifdef _WIN32
	buffer.add_buffer(&buff1);
	buffer.add_buffer(&buff2);
	buffer.add_buffer(&buff3);
	HANDLE tid[THREADS_COUNT];
	DWORD id;

	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	tid[6] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);
	tid[7] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsC, 0, 0, &id);
	tid[8] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsD, 0, 0, &id);
	tid[9] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsE, 0, 0, &id);
	tid[10] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsF, 0, 0, &id);
	tid[11] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsG, 0, 0, &id);
	tid[12] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsH, 0, 0, &id);

	for (int i = 0; i <= THREADS_COUNT; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else

	buffer.add_buffer(&buff1);
	buffer.add_buffer(&buff2);
	buffer.add_buffer(&buff3);
	pthread_t tid[THREADS_COUNT];

	// pthread_create(&(tid[0]), NULL, threadProd, NULL);
	// pthread_create(&(tid[1]), NULL, threadConsA, NULL);
	// pthread_create(&(tid[2]), NULL, threadProd, NULL);
	// pthread_create(&(tid[3]), NULL, threadConsB, NULL);
	// pthread_create(&(tid[4]), NULL, threadConsC, NULL);
	// pthread_create(&(tid[5]), NULL, threadProd, NULL);
	// pthread_create(&(tid[6]), NULL, threadConsD, NULL);
	// pthread_create(&(tid[7]), NULL, threadConsE, NULL);
	// pthread_create(&(tid[8]), NULL, threadConsF, NULL);
	// pthread_create(&(tid[9]), NULL, threadProd, NULL);
	// pthread_create(&(tid[10]), NULL, threadConsG, NULL);
	// pthread_create(&(tid[11]), NULL, threadConsH, NULL);
	// pthread_create(&(tid[12]), NULL, threadProd, NULL);
	pthread_create(&(tid[0]), NULL, threadProd, NULL);
	pthread_create(&(tid[1]), NULL, threadCons, &tArgA);
	pthread_create(&(tid[2]), NULL, threadProd, NULL);
	pthread_create(&(tid[3]), NULL, threadCons, &tArgB);
	pthread_create(&(tid[4]), NULL, threadCons, &tArgC);
	pthread_create(&(tid[5]), NULL, threadProd, NULL);
	pthread_create(&(tid[6]), NULL, threadCons, &tArgD);
	pthread_create(&(tid[7]), NULL, threadCons, &tArgE);
	pthread_create(&(tid[8]), NULL, threadCons, &tArgF);
	pthread_create(&(tid[9]), NULL, threadProd, NULL);
	pthread_create(&(tid[10]), NULL, threadCons, &tArgG);
	pthread_create(&(tid[11]), NULL, threadCons, &tArgH);
	pthread_create(&(tid[12]), NULL, threadProd, NULL);

	for (int i = 0; i < THREADS_COUNT; i++)
		pthread_join(tid[i], (void**)NULL);
	
#endif
	return 0;
}
