#include "Response.hpp"
#include "Status_Handler.hpp"

#include <gtest/gtest.h>
#include <memory>

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
    
    std::string response_body = "Hello World! My payload includes a trailing CRLF.\r\n";
    response->set_body(response_body);
    response->set_content_type("text/html");
    response->set_body_length(response_body.size());
    Status_Handler::handle_status_code(response, 200);

    std::string expected_result =
    {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 51\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "Hello World! My payload includes a trailing CRLF.\r\n",
    };

    ASSERT_EQ(response->get_response_message(), expected_result);
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

TEST(response_tests, parse_content_types)
{
    Message::Response response;

    response.parse_content_type("/demo.txt");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "text/plain");

    response.parse_content_type("/demo.html");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "text/html");

    response.parse_content_type("/demo.css");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "text/css");

    response.parse_content_type("/demo.jpeg");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    response.parse_content_type("/demo.jpg");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    response.parse_content_type("/demo.png");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/png");

    response.parse_content_type("/demo.gif");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/gif");

    response.parse_content_type("/demo.svg");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/svg+xml");

    response.parse_content_type("/demo.ico");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "image/x-icon");

    response.parse_content_type("/demo.json");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/json");

    response.parse_content_type("/demo.pdf");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/pdf");

    response.parse_content_type("/demo.js");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/javascript");

    response.parse_content_type("/demo.xml");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/xml");

    response.parse_content_type("/demo.wasm");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/wasm");

    response.parse_content_type("/demo.xhtml");
    response.add_header("Content-Type", response.get_content_type());
    ASSERT_EQ(response.get_header("Content-Type"), "application/xhtml+xml");
};
