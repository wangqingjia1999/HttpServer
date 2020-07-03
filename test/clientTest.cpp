#include <gtest/gtest.h>
#include "Client.hpp"
#include <thread>

TEST(ClientTests, generateRequestTest)
{
    Client client;

    ASSERT_TRUE(client.parse_uri("http://localhost:2334/"));
    ASSERT_TRUE(client.generate_request());

    std::string generated_request = 
    {
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "User-Agent: Bitate\r\n"
        "\r\n"
    };

    ASSERT_EQ(client.get_generated_request(), generated_request);
    
}

TEST(ClientTests, connectToSpecificServer)
{
    Client client;
    ASSERT_TRUE(client.parse_uri("http://localhost:2333"));
    ASSERT_TRUE(client.connect_to());
    ASSERT_TRUE(client.generate_request()); 
    ASSERT_TRUE(client.send_request());
}