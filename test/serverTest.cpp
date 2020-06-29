#include <gtest/gtest.h>
#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Client.hpp"

TEST(ServerTests, sendResponse)
{
    Server server;
    Client client;
    
    ASSERT_TRUE(server.listenAt("localhost", 2333));
    ASSERT_TRUE(server.receiveRequest());
    ASSERT_TRUE(server.parseRequest());
    ASSERT_TRUE(server.generateResponse());
    ASSERT_TRUE(server.sendResponse());
} 