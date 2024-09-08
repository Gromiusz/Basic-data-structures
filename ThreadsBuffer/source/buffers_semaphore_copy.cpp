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


int const threadsCounts = 13;  // number of threads 13

int const bufferSize = 10;
int probability;
int delay;

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
	bool do_not_print[8];
	std::vector<bool*> readVec;
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
		case 0:
			std::cout << ANSI_COLOR_RED;
			break;
		
		case 1:
			std::cout << ANSI_COLOR_GREEN;
			break;

		case 2:
			std::cout << ANSI_COLOR_MAGENTA;
			break;
		
		default:
			break;
		}
	}

	void who_read() {
		char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
		bool read[] = {readA, readB, readC, readD, readE, readF, readG, readH};

		for (int i = 0; i < sizeof(letters) / sizeof(letters[0]); ++i) {
			std::cout << (read[i] ? letters[i] : '_');
		}
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
		for(int i=0;i<8;i++){
			do_not_print[i]=true;
		}

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
				print("Producer");
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
			print("Producer");
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

		std::string v = values.front();
		readA = true;
		print("A read");
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("A remove");
			readA = false;
			semA.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readD && readE && readF && readG && readH){
			values.erase(values.begin());
			available_S++;
			print("A remove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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

		std::string v = values.front();
		readB = true;
		print("B read");
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("B remove");
			readB = false;
			semB.v();
			full.v();
			counter--;
		}
		else if (readA && readC && readD && readE && readF && readG && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("B rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("C remove");
			readC = false;
			semC.v();
			full.v();
			counter--;
		}
		else if (readB && readA && readD && readE && readF && readG && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("C rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("D remove");
			readD = false;
			semD.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readE && readF && readG && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("D rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("E remove");
			readE = false;
			semE.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readF && readG && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("E rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("F remove");
			readF = false;
			semF.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readG && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("F rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		print("G read");
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("G remove");
			readG = false;
			semG.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readF && readH)
		{
			available_S++;
			values.erase(values.begin());
			print("G rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
		print("H read");
		if(!special_mess.count(v)){
			semS.v();
			values.erase(values.begin());
			print("H remove");
			readH = false;
			semH.v();
			full.v();
			counter--;
		}
		else if (readB && readC && readA && readD && readE && readF && readG)
		{
			available_S++;
			values.erase(values.begin());
			print("H rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			semA.v(); semB.v(); semC.v(); semD.v(); semE.v(); semF.v(); semG.v(); semH.v();
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
	std::vector<int> fullBuffs;
	Semaphore getSem;
	int buffIndexes;
	int visit_left;
	std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};

	BufferCollection(): visit_left(0), getSem(1), buffIndexes(0) {};

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

	int put(std::string value) // was void
	{
		getSem.p();
		int selectBufferPut = -1;
		int tryCounter = 0;
		int result = -1;

		fullBuffs.clear();
		
				for(const auto& buff: bufferCollection){
					if(buff->is_full()){
						fullBuffs.push_back(buff->buffIndex);
					}
				}
		selectBufferPut = get_random_buffer(bufferCollection.size()-1, fullBuffs, value); // wybor tylko z niepelnych buforow

		if(selectBufferPut!=-2){
			bufferCollection[selectBufferPut]->put(value); // ma S1 do wgrania ale bufor nie ma S i jest pelny
			result = 0;
		}
		// int selectBufferPut = -1;
		// int tryCounter = 0;
		// int result = -1;
		// do{
		// 	selectBufferPut = get_random_buffer(bufferCollection.size()-1);
		// 	tryCounter++;
		// }while(bufferCollection[selectBufferPut]->is_full() && tryCounter<10);
		// if(tryCounter<10){
		// 	bufferCollection[selectBufferPut]->put(value);
		// 	result = 0;
		// }
		getSem.v();
		return result;
	}

	template<typename Getter, typename TimesVec>
	std::string get(Getter getter, TimesVec& timesVec)
	{
		getSem.p();
		int selectedBuffer=-1;
		std::string result;
		selectedBuffer = get_random_buffer(bufferCollection.size()-1, timesVec);
		if(selectedBuffer==-2) // jezeli polaczymy z kolejnym if'em to bedziemy sprawdzac rowniez buff[-2]
		{
			result = "Err";
			getSem.v();
			return result;
		}
		if(bufferCollection[selectedBuffer]->is_empty())
		{
			result = "Err";
			getSem.v();
			return result;
		}
		timesVec.push_back(selectedBuffer);
		
		int vol1 = bufferCollection[selectedBuffer]->get_buff_size();
		result = (bufferCollection[selectedBuffer]->*getter)();
		int vol2 = bufferCollection[selectedBuffer]->get_buff_size();
		if(vol2<vol1)
			if(!special_mess.count(result))
				timesVec.erase(std::remove_if(timesVec.begin(), timesVec.end(), [selectedBuffer](int num) {return num == selectedBuffer;}), timesVec.end());
			else
				clear_times(selectedBuffer);
		// int selectedBuffer=-1;
		// int tries = 0;
		// do{
		// 	do{
		// 		selectedBuffer = get_random_buffer(bufferCollection.size()-1);
		// 		tries++;
		// 		if(tries>20){
		// 			std::string result = "Err";
		// 			getSem.v();
		// 			return result;
		// 		}
		// 	}while (std::find(timesVec.begin(), timesVec.end(), selectedBuffer) != timesVec.end());
		// 	tries++;
		// 	if(tries>20){
		// 		std::string result = "Err";
		// 		getSem.v();
		// 		return result;
		// 	}

		// }while(bufferCollection[selectedBuffer]->is_empty());
		// timesVec.push_back(selectedBuffer);
		
		// int vol1 = bufferCollection[selectedBuffer]->get_buff_size();
		// std::string result = (bufferCollection[selectedBuffer]->*getter)();
		// int vol2 = bufferCollection[selectedBuffer]->get_buff_size();
		// if(vol2<vol1)
		// 	clear_times(selectedBuffer);
		// // return (bufferCollection[selectedBuffer]->*getter)();
		getSem.v();
		return result;
	}

	int get_random_buffer(int max, const std::vector<int>& excluded)
	{
		std::vector<int> available_numbers;
		for (int i = 0; i <= max; ++i)
		{
			if (std::find(excluded.begin(), excluded.end(), i) == excluded.end())
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

	int get_random_buffer(int max, const std::vector<int>& excluded, std::string value)
	{
		std::vector<int> available_numbers;
		for (int i = 0; i <= max; ++i)
		{
			if (std::find(excluded.begin(), excluded.end(), i) == excluded.end())
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
	// int get_random_buffer(int max)
	// {
	// 	std::random_device rd;
    // 	std::mt19937 gen(rd());
    // 	std::uniform_int_distribution<> selectBuffer(0, max);
	// 	return selectBuffer(gen);
	// }
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
	{
		auto value = buffer.get(&Buffer::getA, buffer.A_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsB(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getB, buffer.B_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsC(void* arg)
{
	while (true)
	{
		auto value = buffer.get(&Buffer::getC, buffer.C_times);
		if(buffer.areAllBuffersEmpty() && lastMess==5)
			break;
	}
	return NULL;
}

void* threadConsD(void* arg)
{
	while (true)
	{
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
	HANDLE tid[threadsCounts];
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

	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else

	buffer.add_buffer(&buff1);
	buffer.add_buffer(&buff2);
	buffer.add_buffer(&buff3);
	pthread_t tid[threadsCounts];

	pthread_create(&(tid[0]), NULL, threadProd, NULL);
	pthread_create(&(tid[1]), NULL, threadConsA, NULL);
	pthread_create(&(tid[2]), NULL, threadProd, NULL);
	pthread_create(&(tid[3]), NULL, threadConsB, NULL);
	pthread_create(&(tid[4]), NULL, threadConsC, NULL);
	pthread_create(&(tid[5]), NULL, threadProd, NULL);
	pthread_create(&(tid[6]), NULL, threadConsD, NULL);
	pthread_create(&(tid[7]), NULL, threadConsE, NULL);
	pthread_create(&(tid[8]), NULL, threadConsF, NULL);
	pthread_create(&(tid[9]), NULL, threadProd, NULL);
	pthread_create(&(tid[10]), NULL, threadConsG, NULL);
	pthread_create(&(tid[11]), NULL, threadConsH, NULL);
	pthread_create(&(tid[12]), NULL, threadProd, NULL);

	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
	
#endif
	return 0;
}
