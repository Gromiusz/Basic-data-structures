#!/bin/sh

rm -r build
mkdir build
cd build
cmake ..
make
./TEST-GT
./MAIN