cmake_minimum_required(VERSION 3.10)
project(List)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}-ut test.cpp)
add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME}-ut PUBLIC GTest::gtest_main)
target_link_libraries(${PROJECT_NAME}-ut PUBLIC List-lib)
target_link_libraries(${PROJECT_NAME} PUBLIC List-lib)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_include_directories(${PROJECT_NAME}-ut PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_include_directories(List-lib PUBLIC "${PROJECT_SOURCE_DIR}/lib")

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)