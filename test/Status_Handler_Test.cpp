#include "Status_Handler.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(status_handler_tests, status_code_100_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 100);
    
    std::string expected_response_string = {
        "HTTP/1.1 100 Continue\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_101_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 101);
    
    std::string expected_response_string = {
        "HTTP/1.1 101 Switching Protocol\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_200_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 200);
    
    std::string expected_response_string = {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 0\r\n"
	    "Date: \r\n"
        "\r\n"
    };

    // Date string is exactlly 29 character
    ASSERT_EQ(response->get_header("Date").length(), 29);
    ASSERT_EQ(response->get_header("Content-Length"), "0");
}

TEST(status_handler_tests, status_code_201_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 201);
    
    std::string expected_response_string = {
        "HTTP/1.1 201 Created\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_202_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 202);
    
    std::string expected_response_string = {
        "HTTP/1.1 202 Accepted\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_203_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 203);
    
    std::string expected_response_string = {
        "HTTP/1.1 203 Non-Authoritative Information\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_204_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 204);
    
    std::string expected_response_string = {
        "HTTP/1.1 204 No Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_205_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 205);
    
    std::string expected_response_string = {
        "HTTP/1.1 205 Reset Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_206_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 206);
    
    std::string expected_response_string = {
        "HTTP/1.1 206 Partial Content\r\n"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}

TEST(status_handler_tests, status_code_301_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    Status_Handler::handle_status_code(response, 301);
    
    // https://en.wikipedia.org/wiki/HTTP_301
    std::string expected_response_string = {
        "HTTP/1.1 301 Moved Permanently"
        "\r\n"
    };
    
    ASSERT_EQ(response->get_response_message(), expected_response_string);
}