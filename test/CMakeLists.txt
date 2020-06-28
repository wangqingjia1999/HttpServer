cmake_minimum_required(VERSION 3.0.0)

set(this HttpServerTest)

enable_testing()

set(Source
    UriTest.cpp
    serverTest.cpp
    clientTest.cpp
    requestTest.cpp
    responseTest.cpp
    characterSetTest.cpp
    percentEncodingTest.cpp
)

add_executable(${this} ${Source})

target_include_directories(${this} PUBLIC ../include)

target_link_libraries(${this} PUBLIC
    gtest_main 
    HTTP
)


