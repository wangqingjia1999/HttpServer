#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"

#include <gtest/gtest.h>
#include <memory>

TEST(socket_interface_tests, listen_at_no_exception_test)
{
    std::unique_ptr< ISocket > server_socket = std::make_unique< Server_Socket >();
    EXPECT_NO_THROW( server_socket->listen_at("192.112.123.121", 2333) );
}

TEST(socket_interface_tests, connect_to_no_exception_test)
{
    std::unique_ptr< ISocket > client_socket = std::make_unique< Client_Socket >();
    // EXPECT_NO_THROW( client_socket->connect_to() );
}