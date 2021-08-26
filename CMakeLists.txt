cmake_minimum_required(VERSION 3.16)

set(this Http_Server)

project(${this})

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(googletest)

