cmake_minimum_required(VERSION 3.0.0)

set(this HttpServerTest)

enable_testing()

set(SouceFiles
    uriTest.cpp
    serverTest.cpp
    clientTest.cpp
    requestTest.cpp
    responseTest.cpp
    characterSetTest.cpp
    percentEncodingTest.cpp
)

include_directories(${this} PUBLIC ../include)

add_executable(${this} ${SouceFiles})
target_link_libraries(${this} PUBLIC
    gtest_main 
    HTTP
)

# CharacterSet test 
add_executable(CharacterSetTest characterSetTest.cpp)
target_link_libraries(CharacterSetTest PUBLIC
    gtest_main 
    HTTP
)

# PercentEncoding test 
add_executable(PercentEncodingTest percentEncodingTest.cpp)
target_link_libraries(PercentEncodingTest PUBLIC
    gtest_main 
    HTTP
)

# Uri test 
add_executable(UriTest uriTest.cpp)
target_link_libraries(UriTest PUBLIC
    gtest_main 
    HTTP
)

# Request test 
add_executable(RequestTest requestTest.cpp)
target_link_libraries(RequestTest PUBLIC
    gtest_main 
    HTTP
)

# Response test 
add_executable(ResponseTest responseTest.cpp)
target_link_libraries(ResponseTest PUBLIC
    gtest_main 
    HTTP
)

# Server test 
add_executable(ServerTest serverTest.cpp)
target_link_libraries(ServerTest PUBLIC
    gtest_main 
    HTTP
)

# Client test 
add_executable(ClientTest clientTest.cpp)
target_link_libraries(ClientTest PUBLIC
    gtest_main 
    HTTP
)