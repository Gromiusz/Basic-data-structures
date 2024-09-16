cmake_minimum_required(VERSION 3.20)
project(Deque)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include(GoogleTest)

include_directories(lib)
add_library(${PROJECT_NAME}-lib STATIC lib/deque_lib.hpp lib/deque_lib.cpp)

add_executable(${PROJECT_NAME}-ut test/test.cpp)
add_executable(${PROJECT_NAME}-main src/main.cpp)

target_link_libraries(${PROJECT_NAME}-ut PUBLIC GTest::gtest_main ${PROJECT_NAME}-lib)
target_link_libraries(${PROJECT_NAME}-main PUBLIC ${PROJECT_NAME}-lib)

add_test(Test ${PROJECT_NAME}-ut)

gtest_discover_tests(${PROJECT_NAME}-ut)

