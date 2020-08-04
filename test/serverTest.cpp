#include <gtest/gtest.h>

#include "Server.hpp"
#include "Client.hpp"

#include <string>



TEST(server_tests, response_400_bad_request)
{
    Server server;

    std::string raw_request = 
    {
        "GET /lalalalal.html HTTP/1.1\r\n"
    };

    server.set_raw_request(raw_request);
    ASSERT_TRUE(server.parse_request());

    std::string expected_response = 
    {
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 400 Bad Request :( </h1></html>\r\n"
    };

    ASSERT_TRUE(server.generate_response());

    ASSERT_EQ(server.get_raw_response(), expected_response);
}

TEST(server_tests, response_404_not_found)
{
    Server server;

    std::string raw_request = 
    {
        "GET /lalalalal.html HTTP/1.1\r\n"
        "\r\n"
    };

    server.set_raw_request(raw_request);
    ASSERT_TRUE(server.parse_request());

    std::string expected_response = 
    {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 404 Not Found :( </h1></html>\r\n"
    };

    ASSERT_TRUE(server.generate_response());

    ASSERT_EQ(server.get_raw_response(), expected_response);
}

TEST(server_tests, websocket_opening_handshake_test)
{
    Server server;

    std::string raw_request = {
        "GET / HTTP/1.1\r\n"
        "Host: 192.168.72.128:2333\r\n"
        "Connection: Upgrade\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache\r\n"
        "Upgrade: websocket\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: en,zh-CN;q=0.9,zh;q=0.8\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "Sec-WebSocket-Key: oiZnGrVMJ6Ayw6QWFCUeuw==\r\n"
        "Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n"
        "\r\n"
    };

    server.set_raw_request(raw_request);
    ASSERT_TRUE(server.parse_request());

}
