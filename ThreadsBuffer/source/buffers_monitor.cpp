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
	Monitor m;
	Condition empty;
	Condition full;
	Condition conA;
	Condition conB;
	Condition conC;
	Condition conD;
	Condition conE;
	Condition conF;
	Condition conG;
	Condition conH;
	int available_S;
	bool readA = false;
	bool readB = false;
	bool readC = false;
	bool readD = false;
	bool readE = false;
	bool readF = false;
	bool readG = false;
	bool readH = false;
	std::vector<bool*> readVec;
	static Monitor printMon;
	

	// std::vector<std::string> values;
	std::set<std::string> special_mess = {"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8"};

	void print(std::string name)
	{
		printMon.enter();
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
		printMon.leave();
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
	Buffer()
	{
		available_S = 8;
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
		if(values.size()==0){
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
		m.enter();
		if(is_full()) {
			full.wait();
		}
		if(special_mess.count(value)) {
			if(available_S>0){
				available_S--;
				values.push_back(value);
				print("Producer");
				empty.signal();
				m.leave();
			}
			else{
				empty.signal();
				m.leave();
			}
		}
		else
		{
			values.push_back(value);
			print("Producer");
			empty.signal();
			m.leave();
		}
	}


	// std::string getA()
	// {
	// 	monA.enter();
	// 	m.enter();
	// 	if(is_empty()){
	// 		empty.wait();
	// 	}
	// 	// read element from buffer (without removing)
	// 	std::string v = values.front();
	// 	readA = true;
	// 	print("A read");
	// 	if(special_mess.count(v)){
	// 		available_S++;
	// 		values.erase(values.begin());
	// 		print("A remove");
	// 		readA = false;
	// 		monA.leave();
	// 		full.signal();
	// 	}
	// 	else if (readB && readC && readD && readE && readF && readG && readH){
	// 		// remove element from buffer
	// 		values.erase(values.begin());
	// 		print("A remove");
	// 		readA = readB = readC = readD = readE = readF = readG = readH = false;
	// 		monA.leave();
	// 		monB.leave();
	// 		monC.leave();
	// 		monD.leave();
	// 		monE.leave();
	// 		monF.leave();
	// 		monG.leave();
	// 		monH.leave();
	// 		full.signal();
	// 	}
	// 	else{
	// 		empty.signal();
	// 	}
	// 	m.leave();
	// 	return v;
	// }
	std::string getA()
	{
		m.enter();
		if(readA){
			conA.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readA = true;
		print("A read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("A remove");
			readA = false;
			conA.signal();
			full.signal();
		}
		else if (readB && readC && readD && readE && readF && readG && readH){
			// remove element from buffer
			values.erase(values.begin());
			print("A remove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else{
			empty.signal();
		}
		m.leave();
		return v;
	}

	std::string getB()
	{
		m.enter();
		if(readB){
			conB.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readB = true;
		print("B read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("B remove");
			readB = false;
			conB.signal();
			full.signal();
		}
		else if (readA && readC && readD && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("B rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getC()
	{
		m.enter();
		if(readC){
			conC.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readC = true;
		print("C read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("C remove");
			readC = false;
			conC.signal();
			full.signal();
		}
		else if (readB && readA && readD && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("C rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getD()
	{
		m.enter();
		if(readD){
			conD.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readD = true;
		print("D read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("D remove");
			readD = false;
			conD.signal();
			full.signal();
		}
		else if (readB && readC && readA && readE && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("D rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getE()
	{
		m.enter();
		if(readE){
			conE.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readE = true;
		print("E read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("E remove");
			readE = false;
			conE.signal();
			full.signal();
		}
		else if (readB && readC && readA && readD && readF && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("E rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getF()
	{
		m.enter();
		if(readF){
			conF.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readF = true;
		print("F read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("F remove");
			readF = false;
			conF.signal();
			full.signal();
		}
		else if (readB && readC && readA && readD && readE && readG && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("F rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getG()
	{
		m.enter();
		if(readG){
			conG.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readG = true;
		print("G read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("G remove");
			readG = false;
			conG.signal();
			full.signal();
		}
		else if (readB && readC && readA && readD && readE && readF && readH)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("G rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
		return v;
	}

	std::string getH()
	{
		m.enter();
		if(readH){
			conH.wait();
		}
		if(is_empty()){
			empty.wait();
		}
		// read element from buffer (without removing)
		std::string v = values.front();
		readH = true;
		print("H read");
		if(special_mess.count(v)){
			available_S++;
			values.erase(values.begin());
			print("H remove");
			readH = false;
			conH.signal();
			full.signal();
		}
		else if (readB && readC && readA && readD && readE && readF && readG)
		{
			// remove element from buffer
			values.erase(values.begin());
			print("H rmove");
			readA = readB = readC = readD = readE = readF = readG = readH = false;
			conA.signal(); conB.signal(); conC.signal(); conD.signal(); conE.signal(); conF.signal(); conG.signal(); conH.signal();
			full.signal();
		}
		else
			empty.signal();
		m.leave();
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
	Monitor collMon;
	int buffIndexes;

	int visit_left;

	BufferCollection(): visit_left(0), buffIndexes(0) {};

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

	void put(std::string value)
	{
		collMon.enter();
		int selectBufferPut = -1;
		int tryCounter = 0;
		do{
			selectBufferPut = get_random_buffer(bufferCollection.size()-1);
			tryCounter++;
		}while(bufferCollection[selectBufferPut]->is_full() && tryCounter<3);
		if(tryCounter<3){
			bufferCollection[selectBufferPut]->put(value);
		}
		collMon.leave();
	}

	template<typename Getter, typename TimesVec>
	std::string get(Getter getter, TimesVec& timesVec)
	{
		collMon.enter();
		int selectedBuffer=-1;
		int tries = 0;
		do{
			do{
				selectedBuffer = get_random_buffer(bufferCollection.size()-1);
				tries++;
				if(tries>20){
					std::string result = "Err";
					collMon.leave();
					return result;
				}
			}while (std::find(timesVec.begin(), timesVec.end(), selectedBuffer) != timesVec.end());
			tries++;
			if(tries>20){
				std::string result = "Err";
				collMon.leave();
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
		collMon.leave();
		return result;
	}

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
Monitor Buffer::printMon;

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
