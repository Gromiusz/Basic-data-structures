g++ -o init_shm init_shm.cpp -lrt -lpthread
g++ -o producer producer.cpp -lrt -lpthread
g++ -o consumer consumer.cpp -lrt -lpthread
g++ -o cleanup_shm cleanup_shm.cpp -lrt -lpthread
