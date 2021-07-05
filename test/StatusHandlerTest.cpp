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

    std::string expected_response
    {
        "HTTP/1.1 100 Continue\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_101_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 101);
    
    std::string expected_response
    {
        "HTTP/1.1 101 Switching Protocol\r\n"
        "Connection: upgrade\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "Upgrade: HTTP/2.0\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_200_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 200);
    
    std::string expected_response
    {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 0\r\n"
	    "Content-Type: \r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };

    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_201_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 201);
    
    std::string expected_response
    {
        "HTTP/1.1 201 Created\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_202_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 202);
    
    std::string expected_response
    {
        "HTTP/1.1 202 Accepted\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_203_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 203);
    
    std::string expected_response
    {
        "HTTP/1.1 203 Non-Authoritative Information\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_204_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 204);
    
    std::string expected_response
    {
        "HTTP/1.1 204 No Content\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_205_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 205);
    
    std::string expected_response
    {
        "HTTP/1.1 205 Reset Content\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_206_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 206);
    
    std::string expected_response
    {
        "HTTP/1.1 206 Partial Content\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()),
        expected_response
    );
}

TEST(status_handler_tests, status_code_301_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 301);
    
    /**
     *  https://en.wikipedia.org/wiki/HTTP_301
     */
    std::string expected_response
    {
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Content-Length: 0\r\n"
        "Server: Bitate\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(
        weed_out_http_date_header(response->generate_response()), 
        expected_response
    );
}

TEST(status_handler_tests, status_code_404_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 404);
    
    std::string expected_response
    {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: 154\r\n"
        "Content-Type: text/html\r\n"
        "Date: Thu, 12 Nov 2020 13:41:37 GMT\r\n"
        "Server: Bitate\r\n"
        "\r\n"
        "<html>"
            "<head>"
                "<title>"
                    "No results"
                "</title>"
            "</head>"
            "<body>"
                "<center>"
                    "<h1>"
                        "No Results Found"
                    "</h1>"
                    "<div>"
                    "<a href=\"/\">Try Another Query :^)</a>"
                    "</div>"
                "</center>"
            "</body>"
        "</html>"
    };

    ASSERT_EQ( 
        weed_out_http_date_header(response->generate_response()),
        weed_out_http_date_header(expected_response)
    );
}

TEST(status_handler_tests, status_code_405_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    StatusHandler::handle_status_code(response, 405);
    
    std::string expected_response
    {
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Allow: GET\r\n"
        "Content-Length: 222\r\n"
        "Content-Type: text/html\r\n"
        "Date: Thu, 12 Nov 2020 13:41:37 GMT\r\n"
        "Server: Bitate\r\n"
        "\r\n"
        "<html>"
            "<head>"
                "<title>"
                    "405 Method Not Allowed"
                "</title>"
            "</head>"
            "<body>"
                "<center>"
                    "<h1> Please ensure the request method is compliant with the requested resource :^) </h1>"
                "</center>"
                "<center>"
                    "<h5> Bitate Server </h5>"
                "</center>"
            "</body>"
        "</html>"
    };

    ASSERT_EQ( 
        weed_out_http_date_header(response->generate_response()),
        weed_out_http_date_header(expected_response)
    );
}