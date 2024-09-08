#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"
#include <random>
#include <set>
#include <algorithm>
#include <chrono> 
#include <atomic> 

std::atomic<bool> allConsThreadsFinished(false);

int const threadsCounts = 13;  // number of threads 13

int const bufferSize = 10;
int probability;

class Buffer
{
private:
	Semaphore mutex;
	Semaphore empty;
	Semaphore full;
	Semaphore semA;
	Semaphore semB;
	Semaphore semC;
	Semaphore semD;
	Semaphore semE;
	Semaphore semF;
	Semaphore semG;
	Semaphore semH;
	Semaphore semS;
	int available_S;
	int counter;
	bool readA = false;
	bool readB = false;
	bool readC = false;
	bool readD = false;
	bool readE = false;
	bool readF = false;
	bool readG = false;
	bool readH = false;
	std::vector<bool*> readVec;
	static Semaphore printSem;
	

	// std::vector<std::string> values;
	std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};

	void print(std::string name)
	{
		printSem.p();
		std::cout << "\n" << buffIndex<<  " ###" << name << " " << values.size() << "[";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] ###\n";
		printSem.v();
	}

public:
	std::vector<std::string> values;
	int buffIndex;
	Buffer() : mutex(1), empty(0), full(bufferSize), semA(1), semB(1), semC(1), semD(1), semE(1), semF(1), semG(1), semH(1), semS(8)
	{
		available_S = 8;
		counter = 0;
		readVec.push_back(&readA);
        readVec.push_back(&readB);
        readVec.push_back(&readC);
        readVec.push_back(&readD);

	}
	int get_buff_size()
	{
		return values.size();
	}
	bool is_empty()
	{
		if(counter==0){
			return true;
		}
		else{
			return false;
		}
	}

	bool is_full()
	{
		if(values.size()==bufferSize){
			return true;
		}
		else{
			return false;
		}
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
				print("P");
				empty.v();
				counter++;
				mutex.v();
			}
			else{
				full.v();
				mutex.v();
			}
		}
		else
		{
			values.push_back(value);
			print("P");
			empty.v();
			counter++;
			mutex.v();
		}
	}


	std::string getA()
	{
		semA.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readA = true;
		print("A read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("A remove");
			readA = false;
			semA.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readD && readE && readF && readG && readH){
			// remove element from buffer
			values.erase(values.begin());
			print("A remove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else{
			empty.v();
		}
		mutex.v();
		return v;
	}

	std::string getB()
	{
		semB.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readB = true;
		print("B read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("B remove");
			readB = false;
			semB.v();
			full.v();
			counter--;
		}
		else if (readA && readC && readD && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("B rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getC()
	{
		semC.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readC = true;
		print("C read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("C remove");
			readC = false;
			semC.v();
			full.v();
			counter--;
		}
		else if (readB && readA && readD && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("C rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getD()
	{
		semD.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readD = true;
		print("D read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("D remove");
			readD = false;
			semD.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("D rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getE()
	{
		semE.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readE = true;
		print("E read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("E remove");
			readE = false;
			semE.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("E rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getF()
	{
		semF.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readF = true;
		print("F read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("F remove");
			readF = false;
			semF.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("F rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getG()
	{
		semG.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readG = true;
		print("D read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("D remove");
			readG = false;
			semG.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readF && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("D rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}

	std::string getH()
	{
		semH.p();
		empty.p();
		mutex.p();
		// read element from buffer (without removing)
		std::string v = values.front();
		readH = true;
		print("D read");
		if(special_mess.count(v)){
			semS.v();
			available_S++;
			values.erase(values.begin());
			print("D remove");
			readH = false;
			semH.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readF && readG)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("D rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v();
			semB.v();
			semC.v();
			semD.v();
			semE.v();
			semF.v();
			semG.v();
			semH.v();
			full.v();
			counter--;
		}
		else
			empty.v();
		mutex.v();
		return v;
	}
};

class BufferCollection{
	std::vector<Buffer*> bufferCollection;
	

public:
	std::vector<int> A_times;
	std::vector<int> B_times;
	std::vector<int> C_times;
	std::vector<int> D_times;
	std::vector<int> E_times;
	std::vector<int> F_times;
	std::vector<int> G_times;
	std::vector<int> H_times;
	Semaphore getSem;
	Semaphore printQueue;
	int buffIndexes;
	// Semaphore semAA;
	// Semaphore semBB;
	// Semaphore semCC;
	// Semaphore semDD;

	int visit_left;
	// int selectedBuffer;

	BufferCollection(): visit_left(0), getSem(1), printQueue(1), buffIndexes(0) {};

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

	void clear_times(int buff_num)
	{
		A_times.erase(std::remove_if(A_times.begin(), A_times.end(), [buff_num](int num) {
        return num == buff_num;}), A_times.end());
		B_times.erase(std::remove_if(B_times.begin(), B_times.end(), [buff_num](int num) {
        return num == buff_num;}), B_times.end());
		C_times.erase(std::remove_if(C_times.begin(), C_times.end(), [buff_num](int num) {
        return num == buff_num;}), C_times.end());
		D_times.erase(std::remove_if(D_times.begin(), D_times.end(), [buff_num](int num) {
        return num == buff_num;}), D_times.end());
		E_times.erase(std::remove_if(E_times.begin(), E_times.end(), [buff_num](int num) {
        return num == buff_num;}), E_times.end());
		F_times.erase(std::remove_if(F_times.begin(), F_times.end(), [buff_num](int num) {
        return num == buff_num;}), F_times.end());
		G_times.erase(std::remove_if(G_times.begin(), G_times.end(), [buff_num](int num) {
        return num == buff_num;}), G_times.end());
		H_times.erase(std::remove_if(H_times.begin(), H_times.end(), [buff_num](int num) {
        return num == buff_num;}), H_times.end());
	}
	// void print_all(int buff_idx, std::string name)
	// {
    //     printQueue.p();
    //     // for(int i=0; i<buffersCount; i++)
    //     // {
    //     //     std::cout << "Buff[" << i << "] ### " << name << " " << values.size() << "[";
    //     //     for (auto v : values)
    //     //         std::cout << v[i] << ",";
    //     //     std::cout << "]###\t";
    //     // }
    //     // std::cout << "\n";
    //     for(int i=0; i<bufferCollection.size(); i++)
    //     {
	// 		if(buff_idx == i)
    //         	std::cout << "Buff[" << i << "] ### " << name << " " << bufferCollection[i]->get_buff_size() << "[";
	// 		else
	// 			std::cout << "Buff[" << i << "] ###  " << bufferCollection[i]->get_buff_size() << "[";
    //         int counter = 0;

    //         if(bufferCollection[i]->get_buff_size()>0)
    //         {
    //             for (auto v : bufferCollection[i]->values)
    //             {
    //                 std::cout << v;
    //                 if (counter < (bufferCollection[i]->get_buff_size() - 1))
    //                     std::cout << ",";
    //                 counter++;
    //             }
    //         }
    //         std::cout << "] ###\t";
    //     }
    //     std::cout << "\n";
    //     printQueue.v();
	// }
	void put(std::string value)
	{
		getSem.p();
		int selectBufferPut = -1;
		int tryCounter = 0;
		do{
			selectBufferPut = get_random_buffer(bufferCollection.size()-1);
			tryCounter++;
		}while(bufferCollection[selectBufferPut]->is_full() && tryCounter<3);
		if(tryCounter<3){
			bufferCollection[selectBufferPut]->put(value);
		}
		getSem.v();
	}
	// template<typename Getter, typename TimesVec>
	// std::string get(Getter getter, TimesVec& timesVec)
	// {
	// 	getSem.p();
	// 	do{
	// 		if(timesVec.size()==bufferCollection.size()){
	// 			timesVec.clear();
	// 		}
	// 		do{
	// 			selectedBuffer = get_random_buffer(bufferCollection.size()-1);
	// 		}while (std::find(timesVec.begin(), timesVec.end(), selectedBuffer) != timesVec.end());
	// 		timesVec.push_back(selectedBuffer);		
	// 	}while(bufferCollection[selectedBuffer]->is_empty());
	// 	getSem.v();
	// 	return (bufferCollection[selectedBuffer]->*getter)();
	// }
	template<typename Getter, typename TimesVec>
	std::string get(Getter getter, TimesVec& timesVec)
	{
		getSem.p();
		int selectedBuffer=-1;
		int tries = 0;
		do{
			// if(timesVec.size()==bufferCollection.size()){
			// 	timesVec.clear();
			// }
			do{
				selectedBuffer = get_random_buffer(bufferCollection.size()-1);
				tries++;
				if(tries>20){
					std::string result = "Err";
					getSem.v();
					return result;
				}
			}while (std::find(timesVec.begin(), timesVec.end(), selectedBuffer) != timesVec.end());
			tries++;
			if(tries>20){
				std::string result = "Err";
				getSem.v();
				return result;
			}

		}while(bufferCollection[selectedBuffer]->is_empty());
		timesVec.push_back(selectedBuffer);
		
		int vol1 = bufferCollection[selectedBuffer]->get_buff_size();
		std::string result = (bufferCollection[selectedBuffer]->*getter)();
		int vol2 = bufferCollection[selectedBuffer]->get_buff_size();
		if(vol2<vol1)
			clear_times(selectedBuffer);
		// return (bufferCollection[selectedBuffer]->*getter)();
		getSem.v();
		return result;
	}
	// std::string getA()
	// {
	// 	int selectBuffer = get_random_buffer(bufferCollection.size()-1);
	// 	return bufferCollection[selectBuffer]->getA();
	// }
	// std::string getB()
	// {
	// 	int selectBuffer = get_random_buffer(bufferCollection.size()-1);
	// 	return bufferCollection[selectBuffer]->getB();
	// }
	// std::string getC()
	// {
	// 	int selectBuffer = get_random_buffer(bufferCollection.size()-1);
	// 	return bufferCollection[selectBuffer]->getC();
	// }
	// std::string getD()
	// {
	// 	int selectBuffer = get_random_buffer(bufferCollection.size()-1);
	// 	return bufferCollection[selectBuffer]->getD();
	// }
	int get_random_buffer(int max)
	{
		std::random_device rd;
    	std::mt19937 gen(rd());
    	std::uniform_int_distribution<> selectBuffer(0, max);
		return selectBuffer(gen);
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
		buffer.put(str);
	}
	lastMessTh.p();
	// if(lastMess==0)
	// {
		// str = "Last";
		// buffer.put(str);
		lastMess++;
	// }
	lastMessTh.v();
	
	return NULL;
}

void* threadConsA(void* arg)
{
	while (true)
	// for (int i = 0; i < 1000000; ++i)
	{
		// auto value = buffer.getA();
		auto value = buffer.get(&Buffer::getA, buffer.A_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsB(void* arg)
{
	while (true)
	// for (int i = 0; i < 160000; ++i)
	{
		// auto value = buffer.getB();
		auto value = buffer.get(&Buffer::getB, buffer.B_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsC(void* arg)
{
	while (true)
	// for (int i = 0; i < 160000; ++i)
	{
		// auto value = buffer.getC();
		auto value = buffer.get(&Buffer::getC, buffer.C_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsD(void* arg)
{
	while (true)
	// for (int i = 0; i < 160000; ++i)
	{
		// auto value = buffer.getD();
		auto value = buffer.get(&Buffer::getD, buffer.D_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsE(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getE, buffer.E_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsF(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getF, buffer.F_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsG(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getG, buffer.G_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsH(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getH, buffer.H_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}



int main(int argc, char *argv[]) {
    if (argc >= 2) {
        probability = std::atoi(argv[1]);
    } else {
        probability = 4;
    }
	
#ifdef _WIN32
	buffer.add_buffer(&buff1);
	buffer.add_buffer(&buff2);
	buffer.add_buffer(&buff3);
	HANDLE tid[threadsCounts];
	DWORD id;

	// create threads
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

	//  for (int i = 0; i < threadsCounts; i++) {
    //     WaitForSingleObject(tid[i], INFINITE);
    //     if (i >= 5)
    //         allConsThreadsFinished = true;
    // }

	// wait for threads to finish
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else

	buffer.add_buffer(&buff1);
	buffer.add_buffer(&buff2);
	buffer.add_buffer(&buff3);
	pthread_t tid[threadsCounts];

	// create threads
	pthread_create(&(tid[0]), NULL, threadProd, NULL);
	pthread_create(&(tid[1]), NULL, threadProd, NULL);
	pthread_create(&(tid[2]), NULL, threadProd, NULL);
	pthread_create(&(tid[3]), NULL, threadProd, NULL);
	pthread_create(&(tid[4]), NULL, threadProd, NULL);
	pthread_create(&(tid[5]), NULL, threadConsA, NULL);
	pthread_create(&(tid[6]), NULL, threadConsB, NULL);
	pthread_create(&(tid[7]), NULL, threadConsC, NULL);
	pthread_create(&(tid[8]), NULL, threadConsD, NULL);
	pthread_create(&(tid[9]), NULL, threadConsE, NULL);
	pthread_create(&(tid[10]), NULL, threadConsF, NULL);
	pthread_create(&(tid[11]), NULL, threadConsG, NULL);
	pthread_create(&(tid[12]), NULL, threadConsH, NULL);

// 	    // wait for threads to finish
//     for (int i = 0; i < threadsCounts; i++) {
//         pthread_join(tid[i], NULL);
//         if (i >= 5)
//             allConsThreadsFinished = true;
//     }
// #endif

//     while (!allConsThreadsFinished) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }
//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     return 0;
// }

	// wait for threads to finish
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
	
#endif
	return 0;
}
