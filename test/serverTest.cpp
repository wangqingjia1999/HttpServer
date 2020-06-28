#include <gtest/gtest.h>

#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"

TEST(ServerTests, sendResponse)
{
    Server server;
    //Message::Request request;
    Message::Response response;

    // while (server.listenAt("localhost", 65530) == true)
    // {
    //     ASSERT_TRUE(server.listenAt("localhost", 65530));

    //     std::string rawRequest = server.receiveRequest();

    //     request.parseRawRequest(rawRequest);

    //     response.setContent(request.getRequestUri());
    //     response.setStatus(200);

    //     ASSERT_TRUE(response.addHeader("Accept-Ranges", "bytes"));
    //     ASSERT_TRUE(response.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT"));
    //     ASSERT_TRUE(response.generateResponse());
    //     ASSERT_TRUE(server.sendResponse(response.getResponseMessage(), response.getResponseMessageLength()));
    // }
} 