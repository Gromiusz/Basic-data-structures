#!/bin/sh
cd ..
./build/init_shm
./build/producer &
./build/producer &
./build/producer &
./build/producer &
./build/producer &
./build/consumer 0 &
./build/consumer 1 &
./build/consumer 2 &
./build/consumer 3 &
./build/consumer 4 &
./build/consumer 5 &
./build/consumer 6 &
./build/consumer 7 &
