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

    std::string generated_response = 
    {
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 400 Bad Request :( </h1></html>\r\n"
    };

    ASSERT_TRUE(server.generate_response());

    ASSERT_EQ(server.get_raw_response(), generated_response);
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

    std::string generated_response = 
    {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 404 Not Found :( </h1></html>\r\n"
    };

    ASSERT_TRUE(server.generate_response());

    ASSERT_EQ(server.get_raw_response(), generated_response);
}
