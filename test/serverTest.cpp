#include <gtest/gtest.h>

#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"

TEST(ServerTests, sendResponse)
{
    Server server;

    ASSERT_TRUE(server.listenAt("localhost", 4321));
    ASSERT_TRUE(server.receiveRequest());
    ASSERT_TRUE(server.parseRequest());
    ASSERT_TRUE(server.generateResponse());
    ASSERT_TRUE(server.sendResponse());
} 