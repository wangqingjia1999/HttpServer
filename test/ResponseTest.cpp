#include "Response.hpp"
#include "StatusHandler.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(response_tests, add_headers) {
    Message::Response response;

    ASSERT_EQ(response.add_header("Date", "Mon, 27 Jul 2009 12:28:53 GMT"), true);
    ASSERT_EQ(response.add_header("Accept-Ranges", "bytes"), true);
    ASSERT_EQ(response.add_header("Content-Type", "text/plain"), true);
    ASSERT_EQ(response.get_header("Date"), "Mon, 27 Jul 2009 12:28:53 GMT");
    ASSERT_EQ(response.get_header("Accept-Ranges"), "bytes");
    ASSERT_EQ(response.get_header("Content-Type"), "text/plain");
}

TEST(response_tests, generate_200_response)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
    
    std::string body = "Hello World! My payload includes a trailing CRLF.\r\n";
    response->set_body(body);
    response->set_content_type("text/html");
    response->set_body_length(body.size());
    StatusHandler::handle_status_code(response, 200);

    std::string expected_result
    {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 51\r\n"
        "Content-Type: text/html\r\n"
        "Date: Sun, 08 Nov 2020 12:25:15 GMT\r\n"
        "\r\n"
        "Hello World! My payload includes a trailing CRLF.\r\n",
    };

    std::cout << response->generate_response() << std::endl;
}

TEST(response_tests, map_status_code_to_reaseon_phrase)
{
    Message::Response response;

    ASSERT_EQ(response.get_status_code_reason_string(100), "Continue");
    ASSERT_EQ(response.get_status_code_reason_string(200), "OK");
    ASSERT_EQ(response.get_status_code_reason_string(307), "Temporary Redirect");
    ASSERT_EQ(response.get_status_code_reason_string(400), "Bad Request");
    ASSERT_EQ(response.get_status_code_reason_string(404), "Not Found");
    ASSERT_EQ(response.get_status_code_reason_string(500), "Internal Server Error");
    ASSERT_EQ(response.get_status_code_reason_string(502), "Bad Gateway");
    ASSERT_EQ(response.get_status_code_reason_string(505), "HTTP Version Not Supported");
}

