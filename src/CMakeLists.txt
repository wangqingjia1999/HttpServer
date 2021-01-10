include_directories(${CMAKE_SOURCE_DIR}/include)

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
    Logger.cpp
    WebSocket.cpp
    Base64.cpp
    SHA1.cpp
    UTF8.cpp
    Status_Handler.cpp
    Thread_Pool.cpp
    Timer.cpp
    Server_Socket.cpp
)

add_library(${this} STATIC
    ${HeaderFiles}
    ${SourceFiles}
)

add_library(Server_Socket_Lib STATIC
    ../include/Server_Socket.hpp
    Server_Socket.cpp
)

add_library(Client_Socket_Lib STATIC
    ../include/Client_Socket.hpp
    Client_Socket.cpp
)

add_library(Character_Set STATIC
    ../include/Character_Set.hpp
    Character_Set.cpp
)

add_library(Percent_Encoding STATIC
    ../include/Percent_Encoding.hpp
    Percent_Encoding.cpp
)
target_link_libraries(Percent_Encoding PUBLIC
    Character_Set
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

add_library(Status_Handler STATIC
    ../include/Status_Handler.hpp
    Status_Handler.cpp
)