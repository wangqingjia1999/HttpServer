cmake_minimum_required(VERSION 3.8)

set(this HTTP)

project(${this})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(HeaderFiles
    ../include/CharacterSet.hpp
    ../include/Client.hpp
    ../include/PercentEncoding.hpp
    ../include/Request.hpp
    ../include/Response.hpp
    ../include/Server.hpp
    ../include/Uri.hpp
    ../include/mysql_handler.hpp
    ../include/logger.hpp
    ../include/websocket.hpp
    ../include/base64.hpp
)

set(SourceFiles
    CharacterSet.cpp
    Client.cpp
    PercentEncoding.cpp
    Request.cpp
    Response.cpp
    Server.cpp
    Uri.cpp
    mysql_handler.cpp
    logger.cpp
    websocket.cpp
    base64.cpp
)

add_library(${this} STATIC
    ${HeaderFiles}
    ${SourceFiles}
)
