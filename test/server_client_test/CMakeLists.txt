add_executable(simple_client
    simple_client.cpp
)
target_link_libraries(simple_client PUBLIC
    Client_Socket_Lib
)

add_executable(simple_server
    simple_server.cpp
)
target_link_libraries(simple_server PUBLIC
    Server_Lib
)