#!/bin/bash
cd ..
./build/init_shm

gdbserver localhost:1234 ./build/producer &
producer_pid1=$!
gdbserver localhost:1235 ./build/producer &
producer_pid2=$!
gdbserver localhost:1236 ./build/producer &
producer_pid3=$!
gdbserver localhost:1237 ./build/producer &
producer_pid4=$!
gdbserver localhost:1238 ./build/producer &
producer_pid5=$!

gdbserver localhost:1240 ./build/consumer 0 &
consumer_pid1=$!
gdbserver localhost:1241 ./build/consumer 1 &
consumer_pid2=$!
gdbserver localhost:1242 ./build/consumer 2 &
consumer_pid3=$!
gdbserver localhost:1243 ./build/consumer 3 &
consumer_pid4=$!
gdbserver localhost:1244 ./build/consumer 4 &
consumer_pid5=$!
gdbserver localhost:1245 ./build/consumer 5 &
consumer_pid6=$!
gdbserver localhost:1246 ./build/consumer 6 &
consumer_pid7=$!
gdbserver localhost:1247 ./build/consumer 7 &
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
