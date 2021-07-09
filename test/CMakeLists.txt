include_directories(${this} PUBLIC ../include)

enable_testing()

set(source_files
    UriTest.cpp
    Utf8Test.cpp
    Sha1Test.cpp
    Base64Test.cpp
    ClientTest.cpp
    LoggerTest.cpp
    RequestTest.cpp
    ResponseTest.cpp
    SentenceTest.cpp
    WebSocketTest.cpp
    ProcessPoolTest.cpp
    ConnectionTest.cpp
    HtmlBuilderTest.cpp
    ServerSocketTest.cpp
    CharacterSetTest.cpp
    StatusHandlerTest.cpp
    SqliteHandlerTest.cpp
    ResourceHandlerTest.cpp
    PercentEncodingTest.cpp
    ServerConfigurationTest.cpp
    WorkerTest.cpp
)

add_executable(all_tests ${source_files})
target_link_libraries(all_tests PUBLIC
    gtest_main 
    all_libs
    sqlite_handler_lib
)

add_executable(character_set_test CharacterSetTest.cpp)
target_link_libraries(character_set_test PUBLIC
    gtest_main 
    character_set_lib
)

add_executable(percent_encoding_test PercentEncodingTest.cpp)
target_link_libraries(percent_encoding_test PUBLIC
    gtest_main 
    percent_encoding_lib
)

add_executable(uri_test UriTest.cpp)
target_link_libraries(uri_test PUBLIC
    gtest_main 
    uri_lib
)

add_executable(request_test RequestTest.cpp)
target_link_libraries(request_test PUBLIC
    gtest_main 
    request_lib
)

add_executable(response_test ResponseTest.cpp)
target_link_libraries(response_test PUBLIC
    gtest_main 
    response_lib
)

add_executable(server_test ServerTest.cpp)
target_link_libraries(server_test PUBLIC
    gtest_main 
    server_lib
)

add_executable(client_test ClientTest.cpp)
target_link_libraries(client_test PUBLIC
    gtest_main 
    client_lib
)

add_executable(mysql_handler_test MysqlHandlerTest.cpp)
target_link_libraries(mysql_handler_test PUBLIC
    gtest_main 
    mysql_handler_lib
)

add_executable(logger_test LoggerTest.cpp)
target_link_libraries(logger_test PUBLIC
    gtest_main 
    gmock_main
    logger_lib
)

add_executable(websocket_test WebSocketTest.cpp)
target_link_libraries(websocket_test PUBLIC
    gtest_main 
    websocket_lib
)

add_executable(base64_test Base64Test.cpp)
target_link_libraries(base64_test PUBLIC
    gtest_main
    base64_lib
)

add_executable(sha1_test Sha1Test.cpp)
target_link_libraries(sha1_test PUBLIC
    gtest_main
    sha1_lib
)

add_executable(utf8_test Utf8Test.cpp)
target_link_libraries(utf8_test PUBLIC
    gtest_main
    utf8_lib    
)

add_executable(status_handler_test StatusHandlerTest.cpp)
target_link_libraries(status_handler_test PUBLIC
    gtest_main
    status_handler_lib
)

add_executable(process_pool_test ProcessPoolTest.cpp)
target_link_libraries(process_pool_test PUBLIC
    gtest_main
    process_pool_lib
)

add_executable(timer_test TimerTest.cpp)
target_link_libraries(timer_test PUBLIC
    gtest_main
    timer_lib
)

add_executable(server_socket_test
    ServerSocketTest.cpp
)
target_link_libraries(server_socket_test PUBLIC
    server_socket_lib
    client_socket_lib
    gtest_main
)

add_executable(server_configuration_test
    ServerConfigurationTest.cpp
)
target_link_libraries(server_configuration_test PUBLIC
    gtest_main
    server_configuration_lib
)

add_executable(resource_handler_test
    ResourceHandlerTest.cpp
)
target_link_libraries(resource_handler_test PUBLIC
    gtest_main
    resource_handler_lib
)

add_executable(connection_test
    ConnectionTest.cpp
)
target_link_libraries(connection_test PUBLIC
    gtest_main
    connection_lib
)

add_executable(sqlite_handler_test
    SqliteHandlerTest.cpp
)
target_link_libraries(sqlite_handler_test PUBLIC
    gtest_main
    sqlite_handler_lib
)

add_executable(html_builder_test
    HtmlBuilderTest.cpp
)
target_link_libraries(html_builder_test PUBLIC
    gtest_main
    html_builder_lib
)

add_executable(sentence_test
    SentenceTest.cpp
)
target_link_libraries(sentence_test PUBLIC
    gtest_main
)

add_executable(word_finder_main
    WordFinderMain.cpp
)
target_link_libraries(word_finder_main PRIVATE
    server_lib
)

add_executable(word_finder_test
    WordFinderTest.cpp
)
target_link_libraries(word_finder_test PUBLIC
    server_lib
)

add_executable(worker_test
    WorkerTest.cpp
)
target_link_libraries(worker_test PUBLIC
    worker_lib
    gtest_main
)