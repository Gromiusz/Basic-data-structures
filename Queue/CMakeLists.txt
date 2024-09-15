cmake_minimum_required(VERSION 3.20)
project(Deque)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GoogleTest)
include(GoogleTest)

add_library(${PROJECT_NAME}-lib STATIC lib/lib.hpp lib/lib.cpp)

add_test(${PROJECT_NAME}-ut test/test.cpp)
add_executable(${PROJECT_NAME}-main src/main.cpp)

target_link_libraries(${PROJECT_NAME}-ut PUBLIC ${PROJECT_NAME}-lib)
target_link_libraries(${PROJECT_NAME}-ut PUBLIC gtest::gtest.h)
target_link_libraries(${PROJECT_NAME}-main PUBLIC ${PROJECT_NAME}-lib)

