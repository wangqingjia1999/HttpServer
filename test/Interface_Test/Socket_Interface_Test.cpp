#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"

#include <gtest/gtest.h>
#include <memory>

TEST(socket_interface_tests, polymorphism_tests)
{
    std::unique_ptr< ISocket > socket_interface_server = std::make_unique< Server_Socket >();

    std::unique_ptr< ISocket > socket_interface_client = std::make_unique< Client_Socket >();
}

TEST(socket_interface_tests, listen_at_no_exception_test)
{
    std::unique_ptr< Server_Socket > server_socket = std::make_unique< Server_Socket >();
    
    try{
        server_socket->listen_at("127.127.127", 2333);
    }
    catch(std::invalid_argument const& error)
    {
        EXPECT_EQ(error.what(), std::string("Can not create a new socket"));
    }
}

TEST(socket_interface_tests, connect_to_no_exception_test)
{
    std::unique_ptr< ISocket > client_socket = std::make_unique< Client_Socket >();
    // EXPECT_NO_THROW( client_socket->connect_to() );
}