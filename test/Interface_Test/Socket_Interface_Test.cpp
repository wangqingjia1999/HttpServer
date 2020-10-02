#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>

void start_stopwatch();
void stop_stopwatch();
unsigned get_stopwatch_result();

TEST(socket_interface_tests, polymorphism_tests)
{
    std::unique_ptr< ISocket > socket_interface_server = std::make_unique< Server_Socket >();

    std::unique_ptr< ISocket > socket_interface_client = std::make_unique< Client_Socket >();
}

TEST(socket_interface_tests, listen_at_no_exception_test)
{
    std::shared_ptr< Server_Socket > server_socket = std::make_shared< Server_Socket >();
    std::shared_ptr< Client_Socket > client_socket = std::make_shared< Client_Socket >();
    std::unique_ptr< Thread_Pool > thread_pool = std::make_unique< Thread_Pool >();
    
    thread_pool->post_task(
        [client_socket]{
            EXPECT_NO_THROW( client_socket->connect_to("0.0.0.0", 2333) );
        }
    );

    thread_pool->post_task(
        [server_socket]{
            EXPECT_NO_THROW( server_socket->listen_at("0.0.0.0", 2333, 10000) );
        }
    );
}

TEST(socket_interface_tests, connect_to_no_exception_test)
{
    std::unique_ptr< ISocket > client_socket = std::make_unique< Client_Socket >();
    // EXPECT_NO_THROW( client_socket->connect_to() );
}