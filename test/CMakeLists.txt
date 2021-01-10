cmake_minimum_required(VERSION 3.8)

set(this Http_Server_All_Test)

enable_testing()

set(source_files
    URI_Test.cpp
    Server_Test.cpp
    Client_Test.cpp
    Request_Test.cpp
    Response_Test.cpp
    Character_Set_Test.cpp
    Percent_Encoding_Test.cpp
    WebSocket_Test.cpp
)

include_directories(${this} PUBLIC ../include)

add_executable(${this} ${source_files})
target_link_libraries(${this} PUBLIC
    gtest_main 
    Http_Server
)

# Character_Set test 
add_executable(Character_Set_Test Character_Set_Test.cpp)
target_link_libraries(Character_Set_Test PUBLIC
    gtest_main 
    Character_Set
)

# Percent-Encoding test 
add_executable(Percent_Encoding_Test Percent_Encoding_Test.cpp)
target_link_libraries(Percent_Encoding_Test PUBLIC
    gtest_main 
    Percent_Encoding
)

# URI test 
add_executable(URI_Test URI_Test.cpp)
target_link_libraries(URI_Test PUBLIC
    gtest_main 
    Http_Server
)

# Request test 
add_executable(Request_Test Request_Test.cpp)
target_link_libraries(Request_Test PUBLIC
    gtest_main 
    Http_Server
)

# Response test 
add_executable(Response_Test Response_Test.cpp)
target_link_libraries(Response_Test PUBLIC
    gtest_main 
    Http_Server
    
)

# Server test 
add_executable(Server_Test Server_Test.cpp)
target_link_libraries(Server_Test PUBLIC
    gtest_main 
    Http_Server
)

# Client test 
add_executable(Client_Test Client_Test.cpp)
target_link_libraries(Client_Test PUBLIC
    gtest_main 
    Http_Server
)


# Mysql_Handler test 
add_executable(Mysql_Handler_Test Mysql_Handler_Test.cpp)
target_link_libraries(Mysql_Handler_Test PUBLIC
    gtest_main 
    Mysql_Handler
)

# logger test 
add_executable(Logger_Test Logger_Test.cpp)
target_link_libraries(Logger_Test PUBLIC
    gtest_main 
    Logger
)

# WebSocket test 
add_executable(WebSocket_Test WebSocket_Test.cpp)
target_link_libraries(WebSocket_Test PUBLIC
    gtest_main 
    Http_Server
)

# base64 test
add_executable(Base64_Test Base64_Test.cpp)
target_link_libraries(Base64_Test PUBLIC
    gtest_main
    Http_Server    
)

# SHA-1 test
add_executable(SHA1_Test SHA1_Test.cpp)
target_link_libraries(SHA1_Test PUBLIC
    gtest_main
    Http_Server    
)

# UTF-8 test
add_executable(UTF8_Test UTF8_Test.cpp)
target_link_libraries(UTF8_Test PUBLIC
    gtest_main
    Http_Server    
)

# status handler test
add_executable(Status_Handler_Test Status_Handler_Test.cpp)
target_link_libraries(Status_Handler_Test PUBLIC
    gtest_main
    Http_Server
)

# thread pool test
add_executable(Thread_Pool_Test Thread_Pool_Test.cpp)
target_link_libraries(Thread_Pool_Test PUBLIC
    Thread_Pool
    gtest_main
)

# timer test
add_executable(Timer_Test Timer_Test.cpp)
target_link_libraries(Timer_Test PUBLIC
    gtest_main
    Timer
)

# Server Socket test
add_executable(Server_Socket_Test
    Server_Socket_Test.cpp
)
target_link_libraries(Server_Socket_Test PUBLIC
    Server_Socket_Lib
    Client_Socket_Lib
    gtest_main
)