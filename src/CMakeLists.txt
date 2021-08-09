include_directories(${CMAKE_SOURCE_DIR}/include)

set(header_files
    ../include/CharacterSet.hpp
    ../include/Client.hpp
    ../include/PercentEncoding.hpp
    ../include/Request.hpp
    ../include/Response.hpp
    ../include/Uri.hpp
    ../include/Logger.hpp
    ../include/Base64.hpp
    ../include/Sha1.hpp
    ../include/Utf8.hpp
    ../include/StatusHandler.hpp
    ../include/Master.hpp
    ../include/Timer.hpp
    ../include/WorkerSocket.hpp
    ../include/ServerConfiguration.hpp
    ../include/ResourceHandler.hpp
    ../include/Connection.hpp
    ../include/SqliteHandler.hpp
    ../include/HtmlBuilder.hpp
    ../include/Sentence.hpp
    ../include/Worker.hpp
    ../include/Channel.hpp
    ../include/UnixDomainHelper.hpp
)

set(source_files
    CharacterSet.cpp
    Client.cpp
    PercentEncoding.cpp
    Request.cpp
    Response.cpp
    Uri.cpp
    Logger.cpp
    Base64.cpp
    Sha1.cpp
    Utf8.cpp
    StatusHandler.cpp
    Master.cpp
    Timer.cpp
    WorkerSocket.cpp
    ServerConfiguration.cpp
    ResourceHandler.cpp
    Connection.cpp
    SqliteHandler.cpp
    HtmlBuilder.cpp
    Sentence.cpp
    Worker.cpp
    Channel.cpp
    UnixDomainHelper.cpp
)

add_library(all_libs STATIC
    ${header_files}
    ${source_files}
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

add_library(worker_socket_lib STATIC
    ../include/WorkerSocket.hpp
    WorkerSocket.cpp
)
target_link_libraries(worker_socket_lib PRIVATE
    logger_lib
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

add_library(master_lib STATIC
    ../include/Master.hpp
    Master.cpp
)
target_link_libraries(master_lib PRIVATE
    worker_lib
    logger_lib
    channel_lib
    worker_socket_lib
    unix_domain_helper_lib
    rt
)

add_library(timer_lib STATIC
    ../include/Timer.hpp
    Timer.cpp
)

add_library(logger_lib STATIC
    ../include/Logger.hpp
    Logger.cpp
)
target_link_libraries(logger_lib PUBLIC
    server_configuration_lib
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
    sentence_lib    
    connection_lib
    compressor_lib
    sqlite_handler_lib
    server_configuration_lib
)

add_library(sqlite_handler_lib STATIC
    ../include/SqliteHandler.hpp
    SqliteHandler.cpp
)

target_link_directories(sqlite_handler_lib PUBLIC
    /usr/include/
)
target_link_libraries(sqlite_handler_lib PUBLIC
    /usr/lib/x86_64-linux-gnu/libsqlite3.so
    logger_lib
    sentence_lib
    server_configuration_lib
)

add_library(html_builder_lib STATIC
    ../include/HtmlBuilder.hpp
    HtmlBuilder.cpp
)
target_link_libraries(html_builder_lib PUBLIC
    sentence_lib
    sqlite_handler_lib
)

add_library(sentence_lib STATIC
    ../include/Sentence.hpp
    Sentence.cpp
)

add_library(worker_lib STATIC
    ../include/Worker.hpp
    Worker.cpp
)
target_link_libraries(worker_lib PUBLIC
    connection_lib
    worker_socket_lib
    resource_handler_lib
    server_configuration_lib
)
target_link_libraries(worker_lib PRIVATE
    rt
    logger_lib
    status_handler_lib
    unix_domain_helper_lib
)

add_library(channel_lib STATIC
    ../include/Channel.hpp
    Channel.cpp
)

add_library(unix_domain_helper_lib STATIC
    ../include/UnixDomainHelper.hpp
    UnixDomainHelper.cpp
)
target_link_libraries(unix_domain_helper_lib PRIVATE
    logger_lib
)

add_library(compressor_lib STATIC
    ../include/Compressor.hpp
    Compressor.cpp
)
# zlib is named as "libz.so" not "zlib"
target_link_libraries(compressor_lib PRIVATE
    libz.so
)