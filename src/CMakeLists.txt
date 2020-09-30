cmake_minimum_required(VERSION 3.8)

set(this HTTP)

project(${this})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(HeaderFiles
    ../include/Character_Set.hpp
    ../include/Client.hpp
    ../include/Percent_Encoding.hpp
    ../include/Request.hpp
    ../include/Response.hpp
    ../include/Server.hpp
    ../include/URI.hpp
    ../include/Mysql_Handler.hpp
    ../include/Logger.hpp
    ../include/WebSocket.hpp
    ../include/Base64.hpp
    ../include/SHA1.hpp
    ../include/UTF8.hpp
    ../include/Status_Handler.hpp
    ../include/Thread_Pool.hpp
    ../include/Timer.hpp
    ../include/ISocket.hpp
    ../include/Server_Socket.hpp
)

set(SourceFiles
    Character_Set.cpp
    Client.cpp
    Percent_Encoding.cpp
    Request.cpp
    Response.cpp
    Server.cpp
    URI.cpp
    Mysql_Handler.cpp
    Logger.cpp
    WebSocket.cpp
    Base64.cpp
    SHA1.cpp
    UTF8.cpp
    Status_Handler.cpp
    Thread_Pool.cpp
    Timer.cpp
)

add_library(${this} STATIC
    ${HeaderFiles}
    ${SourceFiles}
)

# Socket Interface Lib
add_library(Socket_Interface
    ../include/ISocket.hpp
    ../include/Server_Socket.hpp
    Server_Socket.cpp
)
