#include <gtest/gtest.h>
#include "Client.hpp"
#include <thread>

TEST(ClientTests, generateRequestTest)
{
    Client client;

    ASSERT_TRUE(client.parseUri("http://localhost:2333/"));
    ASSERT_TRUE(client.generateRequest());

    std::string generatedRequest = 
    {
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "User-Agent: Bitate\r\n"
        "\r\n"
    };

    ASSERT_EQ(client.getGeneratedRequest(), generatedRequest);
    
}

TEST(ClientTests, connectToSpecificServer)
{
    Client client;
    ASSERT_TRUE(client.connectTo("localhost", 2333));
    ASSERT_TRUE(client.generateRequest());
    ASSERT_TRUE(client.sendRequest());
}