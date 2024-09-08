cmake_minimum_required(VERSION 3.20)
project(ThreadsBuffer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(buffers_semaphore source/buffers_semaphore_new.cpp)