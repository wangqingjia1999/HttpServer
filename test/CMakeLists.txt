cmake_minimum_required(VERSION 3.0.0)

set(this HttpServerTest)

enable_testing()

# Test all classes
set(SouceFiles
    uriTest.cpp
    serverTest.cpp
    clientTest.cpp
    requestTest.cpp
    responseTest.cpp
    characterSetTest.cpp
    percentEncodingTest.cpp
    mysql_handler_test.cpp
    websocket_test.cpp
)

include_directories(${this} PUBLIC ../include)

add_executable(${this} ${SouceFiles})
target_link_libraries(${this} PUBLIC
    gtest_main 
    HTTP
    mysqlcppconn
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


# mysql_handler test 
add_executable(mysql_handler_test mysql_handler_test.cpp)
target_link_libraries(mysql_handler_test PUBLIC
    gtest_main 
    HTTP
    mysqlcppconn
)

# logger test 
add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test PUBLIC
    gtest_main 
    HTTP
    mysqlcppconn
)

# websocket test 
add_executable(websocket_test websocket_test.cpp)
target_link_libraries(websocket_test PUBLIC
    gtest_main 
    HTTP
    mysqlcppconn
)

# base64 test
add_executable(base64_test base64_test.cpp)
target_link_libraries(base64_test PUBLIC
    gtest_main
    HTTP    
)

# SHA-1 test
add_executable(sha1_test sha1_test.cpp)
target_link_libraries(sha1_test PUBLIC
    gtest_main
    HTTP    
)

# UTF-8 test
add_executable(utf8_test Utf8Tests.cpp)
target_link_libraries(utf8_test PUBLIC
    gtest_main
    HTTP    
)

# status handler test
add_executable(status_handler status_handler_test.cpp)
target_link_libraries(status_handler PUBLIC
    gtest_main
    HTTP    
)