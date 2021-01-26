include_directories(${CMAKE_SOURCE_DIR}/include)

set(header_files
    ../include/CharacterSet.hpp
    ../include/Client.hpp
    ../include/PercentEncoding.hpp
    ../include/Request.hpp
    ../include/Response.hpp
    ../include/Server.hpp
    ../include/Uri.hpp
    ../include/Logger.hpp
    ../include/WebSocket.hpp
    ../include/Base64.hpp
    ../include/Sha1.hpp
    ../include/Utf8.hpp
    ../include/StatusHandler.hpp
    ../include/ThreadPool.hpp
    ../include/Timer.hpp
    ../include/ServerSocket.hpp
    ../include/ServerConfiguration.hpp
    ../include/ResourceHandler.hpp
    ../include/Connection.hpp
)

set(source_files
    CharacterSet.cpp
    Client.cpp
    PercentEncoding.cpp
    Request.cpp
    Response.cpp
    Server.cpp
    Uri.cpp
    Logger.cpp
    WebSocket.cpp
    Base64.cpp
    Sha1.cpp
    Utf8.cpp
    StatusHandler.cpp
    ThreadPool.cpp
    Timer.cpp
    ServerSocket.cpp
    ServerConfiguration.cpp
    ResourceHandler.cpp
    Connection.cpp
)

add_library(all_libs STATIC
    ${header_files}
    ${source_files}
)

add_library(websocket_lib STATIC
    ../include/WebSocket.hpp
    WebSocket.cpp
)
target_link_libraries(websocket_lib PUBLIC
    sha1_lib
    base64_lib
    request_lib
    response_lib
)

add_library(sha1_lib STATIC
    ../include/Sha1.hpp
    Sha1.cpp
)

add_library(utf8_lib STATIC
    ../include/Utf8.hpp
    Utf8.cpp
)

add_library(base64_lib STATIC
    ../include/Base64.hpp
    Base64.cpp
)

add_library(mysql_handler_lib STATIC
    ../include/MysqlHandler.hpp
    MysqlHandler.cpp
)

add_library(request_lib STATIC
    ../include/Request.hpp
    Request.cpp
)
target_link_libraries(request_lib PUBLIC
    uri_lib
    logger_lib
)

add_library(server_socket_lib STATIC
    ../include/ServerSocket.hpp
    ServerSocket.cpp
)

add_library(server_lib STATIC
    ../include/Server.hpp
    Server.cpp
)
target_link_libraries(server_lib PUBLIC
    request_lib
    response_lib
    websocket_lib
    thread_pool_lib
    server_socket_lib
    server_configuration_lib
)

add_library(client_socket_lib STATIC
    ../include/ClientSocket.hpp
    ClientSocket.cpp
)

add_library(client_lib STATIC
    ../include/Client.hpp
    Client.cpp
)

add_library(character_set_lib STATIC
    ../include/CharacterSet.hpp
    CharacterSet.cpp
)

add_library(uri_lib STATIC
    ../include/Uri.hpp
    Uri.cpp
)
target_link_libraries(uri_lib
    character_set_lib
    percent_encoding_lib
)

add_library(percent_encoding_lib STATIC
    ../include/PercentEncoding.hpp
    PercentEncoding.cpp
)
target_link_libraries(percent_encoding_lib PUBLIC
    character_set_lib
)

add_library(thread_pool_lib STATIC
    ../include/ThreadPool.hpp
    ThreadPool.cpp
)

add_library(timer_lib STATIC
    ../include/Timer.hpp
    Timer.cpp
)

add_library(logger_lib STATIC
    ../include/Logger.hpp
    Logger.cpp
)

add_library(response_lib STATIC
    ../include/Response.hpp
    Response.cpp
)
target_link_libraries(response_lib PUBLIC
    status_handler_lib
    uri_lib
)

add_library(status_handler_lib STATIC
    ../include/StatusHandler.hpp
    StatusHandler.cpp
)
target_link_libraries(status_handler_lib PUBLIC
    timer_lib
    response_lib
)

add_library(server_configuration_lib
    ../include/ServerConfiguration.hpp
    ServerConfiguration.cpp
)

add_library(connection_lib
    ../include/Connection.hpp
    Connection.cpp
)
target_link_libraries(connection_lib PUBLIC
    request_lib
    response_lib
)

add_library(resource_handler_lib
    ../include/ResourceHandler.hpp
    ResourceHandler.cpp
)
target_link_libraries(resource_handler_lib PUBLIC
    connection_lib
)

