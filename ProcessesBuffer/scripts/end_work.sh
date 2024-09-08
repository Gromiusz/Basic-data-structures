#!/bin/sh

./kill_consumers.sh
./kill_producers.sh
cd ..
cd build
./cleanup_shm