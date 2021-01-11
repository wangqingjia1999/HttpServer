add_executable(echo_client
    echo_client.cpp
)
target_link_libraries(echo_client PUBLIC
    Client_Socket_Lib
)

add_executable(echo_server
    echo_server.cpp
)
target_link_libraries(echo_server PUBLIC
    Server_Socket_Lib
)