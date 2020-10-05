#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>

TEST(socket_interface_tests, polymorphism_tests)
{
    std::unique_ptr< ISocket > socket_interface_server = std::make_unique< Server_Socket >();

    std::unique_ptr< ISocket > socket_interface_client = std::make_unique< Client_Socket >();
}

TEST(socket_interface_tests, server_listen_at_no_exception_test)
{
    std::shared_ptr< Server_Socket > server_socket = std::make_shared< Server_Socket >();
    Thread_Pool tp;
    try{    
        tp.post_task(
            [server_socket]{
                server_socket->listen_at("0.0.0.0", 2333);
            }
        );
    }catch(std::exception& error)
    {
        std::cout << error.what() << std::endl;
    }
}

TEST(socket_interface_tests, connect_to_no_exception_test)
{
    std::unique_ptr< ISocket > client_socket = std::make_unique< Client_Socket >();
    // EXPECT_NO_THROW( client_socket->connect_to() );
}