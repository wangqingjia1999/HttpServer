cmake_minimum_required(VERSION 3.16)

set(this Http_Server)

project(${this})

# Use C++ 11 standard
set(CMAKE_CXX_STANDARD 11)

# Use POSIX threads library, not supported on Windows
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(googletest)
add_subdirectory(test)

