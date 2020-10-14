cmake_minimum_required(VERSION 3.8)

set(this Http_Server_Demo)

project(${this})

# Use C++ 11 standard
set(CMAKE_CXX_STANDARD 11)

# Use POSIX threads library, not supported on Windows
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

enable_testing()

# Use tis command to force google test to link dynamically
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(src)
add_subdirectory(googletest)
add_subdirectory(test)

add_executable(${this} main.cpp)

target_link_libraries(${this} PUBLIC
    HTTP
)
