#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>

// Time units
const u_int64 microseconds = 1;
const u_int64 milliseconds = 1000 * microseconds;
const u_int64 seconds = 1000 * milliseconds;


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

    EXPECT_NO_THROW( server_socket->listen_at(ip, port, 1 * milliseconds) );

    EXPECT_EQ( server_socket->get_current_server_status(), server_status::LISTEN_TIMEOUT );
}