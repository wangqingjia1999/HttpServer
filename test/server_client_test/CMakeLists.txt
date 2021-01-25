add_executable(simple_client
    SimpleClient.cpp
)
target_link_libraries(simple_client PUBLIC
    client_socket_lib
)

add_executable(simple_server
    SimpleServer.cpp
)
target_link_libraries(simple_server PUBLIC
    server_lib
)