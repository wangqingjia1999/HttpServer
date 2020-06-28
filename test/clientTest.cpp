#include <gtest/gtest.h>
#include "Client.hpp"

TEST(ClientTests, generateRequestTest)
{
    Uri::Uri uri;
    Client client;

    ASSERT_TRUE(uri.parseFromString("http://localhost:2333/"));

    ASSERT_TRUE(client.generateRequest());

    std::string generatedRequest = 
    {
        "GET / HTTP/1.1\r\n"
        "Host: localhost:2333\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Accept: */*\r\n"
        "\r\n"
    };

    
}