cmake_minimum_required(VERSION 3.10)
project(BinaryTree)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MAIN main.cpp)

add_subdirectory(treelib)

target_link_libraries(MAIN PUBLIC lib)
target_include_directories(MAIN PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/treelib")

add_executable(TEST-GT test.cpp)
target_link_libraries(TEST-GT PUBLIC lib)
target_include_directories(TEST-GT PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/treelib")

target_link_libraries(TEST-GT PUBLIC GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TEST-GT)

