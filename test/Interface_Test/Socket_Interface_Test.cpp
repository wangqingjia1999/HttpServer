#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>

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

    std::string ip = "0.0.0.0";
    int port = 2333;

    EXPECT_NO_THROW( server_socket->listen_at(ip, port, 1 * milliseconds) );

    EXPECT_EQ( server_socket->get_current_server_status(), Server_Status::CLOSED );
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
                    2333,
                    1 * seconds
                );
            );
        }
    );

    // BUG: client still cannot connects to server,
    // I guess that the client begins to connect before the server starts listening.
    // Namely, tricky multithreading problem.
    thread_pool->post_task(
        [&](){
            if( server_socket->get_current_server_status() == Server_Status::LISTENING )
            {
                EXPECT_NO_THROW(
                    client_socket->connect_to(
                        "127.0.0.1",
                        2333
                    );
                );
            }                
        }
    );

}