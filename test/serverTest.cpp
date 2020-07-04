#include <gtest/gtest.h>
#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <thread>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>


TEST(server_tests, listen_at_test)
{
    Server server;
    ASSERT_TRUE(server.listen_at("127.0.0.1", 2333));
} 

TEST(server_tests, generate_response)
{
    Server server;
    ASSERT_TRUE(server.generate_response());
    ASSERT_EQ(server.get_raw_request(), "");
}



TEST(server_tests, todos)
{
    // Client client;

    // ASSERT_TRUE(server.listen_at("localhost", 2333));
    // ASSERT_TRUE(server.receive_request());
    // ASSERT_TRUE(server.parse_request());
    // ASSERT_TRUE(server.generate_response());
    // ASSERT_TRUE(server.send_response());    
    // epoll_reactor(server);
    
    // ASSERT_TRUE(client.parse_uri("http://localhost:8088/"));
    // ASSERT_TRUE(client.connect_to());
    // ASSERT_TRUE(client.generate_request());
    // ASSERT_TRUE(client.send_request());

    // ASSERT_EQ(client.get_generated_request(), server.get_raw_request());
}