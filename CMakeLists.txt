cmake_minimum_required(VERSION 3.8)

set(this Http_Server_Demo)

# Use C++ 11 standard
set(CMAKE_CXX_STANDARD 11)

# Use POSIX threads library
set(CMAKE_CXX_FLAGS "-pthread")

project(${this})

enable_testing()

add_subdirectory(src)
add_subdirectory(googletest)
add_subdirectory(test)

add_executable(${this} main.cpp)

target_link_libraries(${this} PUBLIC
    HTTP
    mysqlcppconn
)
