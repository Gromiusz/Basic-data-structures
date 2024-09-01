g++ -g -o init_shm init_shm.cpp lib.cpp -lrt -lpthread
g++ -g -o producer producer.cpp lib.cpp -lrt -lpthread
g++ -g -o consumer consumer.cpp lib.cpp -lrt -lpthread
