#!/bin/bash
./build/init_shm

./build/producer &
producer_pid1=$!
./build/producer &
producer_pid2=$!
./build/producer &
producer_pid3=$!
./build/producer &
producer_pid4=$!
./build/producer &
producer_pid5=$!


./build/consumer 0 &
consumer_pid1=$!
./build/consumer 1 &
consumer_pid2=$!
./build/consumer 2 &
consumer_pid3=$!
./build/consumer 3 &
consumer_pid4=$!
./build/consumer 4 &
consumer_pid5=$!
./build/consumer 5 &
consumer_pid6=$!
./build/consumer 6 &
consumer_pid7=$!
./build/consumer 7 &
consumer_pid8=$!

wait $producer_pid1
wait $producer_pid2
wait $producer_pid3
wait $producer_pid4
wait $producer_pid5
wait $consumer_pid1
wait $consumer_pid2
wait $consumer_pid3
wait $consumer_pid4
wait $consumer_pid5
wait $consumer_pid6
wait $consumer_pid7
wait $consumer_pid8

../build/cleanup_shm

echo "All producers and consumers have finished."
