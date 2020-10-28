include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("C:/Program Files/MySQL/Connector C++ 8.0/include")

set(HeaderFiles
    ../include/Character_Set.hpp
    ../include/Client.hpp
    ../include/Percent_Encoding.hpp
    ../include/Request.hpp
    ../include/Response.hpp
    ../include/Server.hpp
    ../include/URI.hpp
    ../include/Logger.hpp
    ../include/WebSocket.hpp
    ../include/Base64.hpp
    ../include/SHA1.hpp
    ../include/UTF8.hpp
    ../include/Status_Handler.hpp
    ../include/Thread_Pool.hpp
    ../include/Timer.hpp
)

set(SourceFiles
    Character_Set.cpp
    Client.cpp
    Percent_Encoding.cpp
    Request.cpp
    Response.cpp
    Server.cpp
    URI.cpp
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

# Here we don't need to include ISocket.hpp
# Since it's just an interface
add_library(Socket_Interface STATIC
    ../include/Server_Socket.hpp
    Server_Socket.cpp
    ../include/Client_Socket.hpp
    Client_Socket.cpp
)

add_library(Thread_Pool STATIC
    ../include/Thread_Pool.hpp
    Thread_Pool.cpp
)

add_library(Timer STATIC
    ../include/Timer.hpp
    Timer.cpp
)

add_library(Logger STATIC
    ../include/Logger.hpp
    Logger.cpp
)

add_library(Response STATIC
    ../include/Response.hpp
    Response.cpp
)

add_library(Mysql_Handler STATIC
    ../include/Mysql_Handler.hpp
    Mysql_Handler.cpp
)
target_link_libraries(Mysql_Handler PUBLIC
    "C:/Program Files/MySQL/Connector C++ 8.0/lib64/vs14/mysqlcppconn8-static.lib"
) 

