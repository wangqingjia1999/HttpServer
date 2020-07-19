cmake_minimum_required(VERSION 3.8)

set(this HttpServerExe)

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
