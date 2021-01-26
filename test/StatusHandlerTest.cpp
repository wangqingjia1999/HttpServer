#include "StatusHandler.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace
{
    /**
     * Weed out dynamically generated http date to facilitate the checking of two http messages.
     * 
     * @param[in] message
     *      Http request/response message.
     * 
     * @return
     *      Http request/response message without date header.
     */
    inline std::string weed_out_http_date_header(const std::string& message)
    {
        // Weed out: "Date: Thu, 12 Nov 2020 13:41:37 GMT\r\n"
        if(message.find("Date:") != std::string::npos && message.find("GMT") != std::string::npos)
        {
            std::string before = message.substr(0, message.find("Date:"));
            std::string after = message.substr(message.find("GMT")+5);
            return before + after;
        }
        return message;
    }
}

TEST(status_handler_tests, status_code_100_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 100);
    
    std::string expected_response_string
    {
        "HTTP/1.1 100 Continue\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_101_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 101);
    
    std::string expected_response_string
    {
        "HTTP/1.1 101 Switching Protocol\r\n"
        "Connection: upgrade\r\n"
        "Upgrade: HTTP/2.0\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_200_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 200);
    
    std::string expected_response_string
    {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 0\r\n"
	    "Content-Type: \r\n"
        "\r\n"
    };

    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_201_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 201);
    
    std::string expected_response_string
    {
        "HTTP/1.1 201 Created\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_202_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 202);
    
    std::string expected_response_string
    {
        "HTTP/1.1 202 Accepted\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_203_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 203);
    
    std::string expected_response_string
    {
        "HTTP/1.1 203 Non-Authoritative Information\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_204_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 204);
    
    std::string expected_response_string
    {
        "HTTP/1.1 204 No Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_205_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 205);
    
    std::string expected_response_string
    {
        "HTTP/1.1 205 Reset Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_206_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 206);
    
    std::string expected_response_string
    {
        "HTTP/1.1 206 Partial Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()),
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_301_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 301);
    
    /**
     *  https://en.wikipedia.org/wiki/HTTP_301
     */
    std::string expected_response_string
    {
        "HTTP/1.1 301 Moved Permanently\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->get_response_message()), 
        expected_response_string
    );
}

TEST(status_handler_tests, status_code_404_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 404);
    
    std::string expected_response_string
    {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: 135\r\n"
        "Content-Type: text/html\r\n"
        "Date: Thu, 12 Nov 2020 13:41:37 GMT\r\n"
        "\r\n"
        "<html>"
            "<head>"
                "<title>"
                    "404 Not Found"
                "</title>"
            "</head>"
            "<body>"
                "<center>"
                    "<h1>404 Not Found</h1>"
                "</center>"
                "<center>"
                    "Bitate/0.0.1"
                "</center>"
            "</body>"
        "</html>"
    };

    ASSERT_EQ( 
        weed_out_http_date_header(response->get_response_message()),
        weed_out_http_date_header(expected_response_string)
    );
}