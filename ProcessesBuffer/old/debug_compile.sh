g++ -g init_shm.cpp lib.cpp -o init_shm -lrt -lpthread
g++ -g producer.cpp lib.cpp -o producer -lrt -lpthread
g++ -g consumer.cpp lib.cpp -o consumer -lrt -lpthread
g++ -g cleanup_shm.cpp lib.cpp -o cleanup_shm -lrt -lpthread
