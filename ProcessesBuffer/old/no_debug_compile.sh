g++ init_shm.cpp lib.cpp -o init_shm -lrt -lpthread
g++ producer.cpp lib.cpp -o producer -lrt -lpthread
g++ consumer.cpp lib.cpp -o consumer -lrt -lpthread
g++ cleanup_shm.cpp lib.cpp -o cleanup_shm -lrt -lpthread
