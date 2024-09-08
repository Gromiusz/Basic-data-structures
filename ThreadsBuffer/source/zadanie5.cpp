#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"
#include <random>
#include <algorithm>

int const threadsCounts = 4;  // number of threads

int const bufferSize = 9;


class Buffer
{
private:
	Semaphore mutex;
	Semaphore empty1;
	Semaphore full1;
	Semaphore semA1;
	Semaphore semB1;
	// Semaphore mutex2;
	Semaphore empty2;
	Semaphore full2;
	Semaphore semA2;
	Semaphore semB2;
	// Semaphore mutex3;
	Semaphore empty3;
	Semaphore full3;
	Semaphore semA3;
	Semaphore semB3;
    // std::vector<Semaphore> mutex;
    std::vector<Semaphore> empty;
    std::vector<Semaphore> full;
    std::vector<Semaphore> semA;
    std::vector<Semaphore> semB;
    Semaphore getIdx;
    Semaphore printQueue;
    std::vector<bool> readA;
    std::vector<bool> readB;
    int buffersCount;
    int visit_left;
    std::vector<int> visited_buffers;
    int selectedBuffer;
    // int selectedBuffer;
	// bool readA = false;
	// bool readB = false;

	std::vector<std::vector<int>> values;

	void print(int buff_idx, std::string name)
	{
        printQueue.p();
        // for(int i=0; i<buffersCount; i++)
        // {
        //     std::cout << "Buff[" << i << "] ### " << name << " " << values.size() << "[";
        //     for (auto v : values)
        //         std::cout << v[i] << ",";
        //     std::cout << "]###\t";
        // }
        // std::cout << "\n";
        for(int i=0; i<buffersCount; i++)
        {
			if(buff_idx == i)
            	std::cout << "Buff[" << i << "] ### " << name << " " << values[i].size() << "[";
			else
				std::cout << "Buff[" << i << "] ###  " << values[i].size() << "[";
            int counter = 0;

            if(values[i].size()>0)
            {
                for (auto v : values[i])
                {
                    std::cout << v;
                    if (counter < (values[i].size() - 1))
                        std::cout << ",";
                    counter++;
                }
            }
            std::cout << "] ###\t";
        }
        std::cout << "\n";
        printQueue.v();
	}

public:
	Buffer(int quantity): buffersCount(quantity), visit_left(0), getIdx(1), printQueue(1), selectedBuffer(-1), 
		mutex(1), empty1(0), full1(bufferSize), semA1(1), semB1(1),
		empty2(0), full2(bufferSize), semA2(1), semB2(1),
		empty3(0), full3(bufferSize), semA3(1), semB3(1)
	{
        for (int i = 0; i < buffersCount; ++i) {
        //     mutex.push_back(Semaphore(1));
        //     empty.push_back(Semaphore(0));
        //     full.push_back(Semaphore(bufferSize));
        //     values.push_back(std::vector<int>());
        //     semA.push_back(Semaphore(1));
        //     semB.push_back(Semaphore(1));
        //     readA.push_back(false);
        //     readB.push_back(false);
			values.push_back(std::vector<int>());
			readA.push_back(false);
			readB.push_back(false);
        }
		empty.push_back(empty1);
		full.push_back(full1);
		semA.push_back(semA1);
		semB.push_back(semB1);

		empty.push_back(empty2);
		full.push_back(full2);
		semA.push_back(semA2);
		semB.push_back(semB2);

		empty.push_back(empty3);
		full.push_back(full3);
		semA.push_back(semA3);
		semB.push_back(semB3);
	}

    int get_random_buffer_idx()
	{
		std::random_device rd;
    	std::mt19937 gen(rd());
    	std::uniform_int_distribution<> selectBuffer(0, buffersCount-1);
		return selectBuffer(gen);
	}

    int get_synchronized_buffer_idx()
    {
        if(visit_left==0)
		{
			do{
				if(visited_buffers.size()==buffersCount){
					visited_buffers.clear();
				}
				do{
					selectedBuffer = get_random_buffer_idx();
				}while (std::find(visited_buffers.begin(), visited_buffers.end(), selectedBuffer) != visited_buffers.end());
				visited_buffers.push_back(selectedBuffer);
			}while(values[selectedBuffer].size()==0);
			visit_left=2;
		}
		visit_left--;
        return selectedBuffer;
    }

	void put(int value)
	{
        int buff_idx = get_random_buffer_idx();
		full[buff_idx].p();
		mutex.p();
		// insert element to buffer
		values[buff_idx].push_back(value);
		print(buff_idx, "P");
		empty[buff_idx].v();
		mutex.v();
	}

	int getA()
	{
        getIdx.p();
        get_synchronized_buffer_idx();
        int buff_idx = selectedBuffer;
        getIdx.v();
		semA[buff_idx].p();
		empty[buff_idx].p();
		mutex.p();
		// read element from buffer (without removing)
		int v = values[buff_idx].front();
		readA[buff_idx] = true;
		print(buff_idx, "A read");
		if (readB[buff_idx])
		{
			// remove element from buffer
			values.erase(values.begin());
			print(buff_idx, "A remove");
			readA[buff_idx] = readB[buff_idx] = false;
			semA[buff_idx].v();
			semB[buff_idx].v();
			full[buff_idx].v();
		}
		else
			empty[buff_idx].v();
		mutex.v();
		return v;
	}

	int getB()
	{
        getIdx.p();
        get_synchronized_buffer_idx();
        int buff_idx = selectedBuffer;
        getIdx.v();
		semB[buff_idx].p();
		empty[buff_idx].p();
		mutex.p();
		// read element from buffer (without removing)
		int v = values[buff_idx].front();
		readB[buff_idx] = true;
		print(buff_idx, "B read");
		if (readA[buff_idx])
		{
			// remove element from buffer
			values.erase(values.begin());
			print(buff_idx, "B rmove");
			readA[buff_idx] = readB[buff_idx] = false;
			semA[buff_idx].v();
			semB[buff_idx].v();
			full[buff_idx].v();
		}
		else
			empty[buff_idx].v();
		mutex.v();
		return v;
	}
};

Buffer buffer(3);

void* threadProd(void* arg)
{
	for (int i = 0; i < 10; ++i)
	{
		buffer.put(i);
	}
	return NULL;
}

void* threadConsA(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		auto value = buffer.getA();
	}
	return NULL;
}

void* threadConsB(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		auto value = buffer.getB();
	}
	return NULL;
}

int main()
{
#ifdef _WIN32
	HANDLE tid[threadsCounts];
	DWORD id;

	// create threads
	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);

	// wait for threads to finish
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else
	pthread_t tid[threadsCounts];

	// create threads
	pthread_create(&(tid[0]), NULL, threadProd, NULL);
	pthread_create(&(tid[1]), NULL, threadProd, NULL);
	pthread_create(&(tid[2]), NULL, threadConsA, NULL);
	pthread_create(&(tid[3]), NULL, threadConsB, NULL);

	// wait for threads to finish
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
#endif
	return 0;
}
