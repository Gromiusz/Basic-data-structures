cmake_minimum_required(VERSION 3.10)
project(Hashtable)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(HashtabLibrary)


add_executable(main hash_tab.cpp)
target_link_libraries(main PUBLIC HashtabLibrary)
target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/HashtabLibrary")

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test-gt test.cpp)
target_link_libraries(test-gt PUBLIC gtest_main)
target_link_libraries(test-gt PUBLIC HashtabLibrary)
target_include_directories(test-gt PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/HashtabLibrary")
add_test(NAME test COMMAND test-gt)