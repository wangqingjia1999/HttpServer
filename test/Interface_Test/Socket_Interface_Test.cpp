#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>

TEST(socket_interface_tests, polymorphism_tests)
{
    std::unique_ptr< ISocket > socket_interface_server = std::make_unique< Server_Socket >();

    std::unique_ptr< ISocket > socket_interface_client = std::make_unique< Client_Socket >();
}

TEST(socket_interface_tests, server_listen_at_no_exception_test)
{
    auto server_socket = std::make_shared< Server_Socket >();
    auto thread_pool = std::make_shared< Thread_Pool >();

    std::string ip = "0.0.0.0";
    int port = 2333;

    // Bug: the test will abort without any warnings or errors
    thread_pool->post_task(
        [server_socket, ip, port]{
            server_socket->listen_at(ip, port);
        }
    );
}

TEST(socket_interface_tests, connect_to_no_exception_test)
{
    std::unique_ptr< ISocket > client_socket = std::make_unique< Client_Socket >();
    // EXPECT_NO_THROW( client_socket->connect_to() );
}