cmake_minimum_required(VERSION 3.20)
project(ProcessesBuffer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME}-lib STATIC lib/lib.cpp)

add_executable(cleanup_shm src/cleanup_shm.cpp)
add_executable(init_shm src/init_shm.cpp)
add_executable(consumer src/consumer.cpp)
add_executable(producer src/producer.cpp)

target_link_libraries(cleanup_shm PUBLIC ${PROJECT_NAME}-lib)
target_link_libraries(init_shm PUBLIC ${PROJECT_NAME}-lib)
target_link_libraries(consumer PUBLIC ${PROJECT_NAME}-lib)
target_link_libraries(producer PUBLIC ${PROJECT_NAME}-lib)
