#include <gtest/gtest.h>
#include "Client.hpp"

TEST(ClientTests, generateRequestTest)
{
    Client client;

    ASSERT_TRUE(client.generateRequest());

    std::string generatedRequest = 
    {
        "GET / HTTP/1.1"
        "Host: localhost:2333"
        "User-Agent: curl/7.68.0"
        "Accept: */*"
    };

    

}