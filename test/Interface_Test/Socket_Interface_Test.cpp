#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>

/**
 * Incomplete code, just ignore these lines.
 */

// Time units for testing
const u_int64 microseconds = 1;
const u_int64 milliseconds = 1000 * microseconds;
const u_int64 seconds = 1000 * milliseconds;


TEST(socket_interface_tests, polymorphism_tests)
{
    std::unique_ptr< ISocket > socket_interface_server = std::make_unique< Server_Socket >();
    
    std::unique_ptr< ISocket > socket_interface_client = std::make_unique< Client_Socket >();
}

TEST(socket_interface_tests, server_listens_at_no_exception_test)
{
    auto server_socket = std::make_shared< Server_Socket >();
    auto thread_pool = std::make_unique< Thread_Pool >();
    
    std::string ip = "127.0.0.1";
    int port = 23332;

    // I should split thread pool module to a dedicated library.
    thread_pool->post_task(
        [&](){
            EXPECT_NO_THROW( server_socket->listen_at(ip, port) );
        }
    );

    thread_pool->post_task(
        [&](){
            server_socket->stop_listening();
        }
    );
}

TEST(socket_interface_tests, client_connects_to_test)
{
    auto client_socket = std::make_shared< Client_Socket >();

    // 39.156.69.79 is the IPv4 address of baidu.com
    EXPECT_NO_THROW( client_socket->connect_to( "39.156.69.79", 443 ) );
}

TEST(socket_interface_tests, client_sends_request_while_server_response)
{
    auto server_socket = std::make_shared< Server_Socket >();
    auto client_socket = std::make_shared< Client_Socket >();
    auto thread_pool = std::make_unique< Thread_Pool >();

    thread_pool->post_task(
        [&](){
            EXPECT_NO_THROW(
                server_socket->listen_at(
                    "127.0.0.1",
                    2333
                );
            );
        }
    );

    thread_pool->post_task(
        [&](){
            EXPECT_NO_THROW(
                client_socket->connect_to(
                    "127.0.0.1",
                    2333
                );
            );
        }
    );
}