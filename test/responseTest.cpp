#include <gtest/gtest.h>
#include <memory>
#include "Response.hpp"
#include "status_handler.hpp"

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
    status_handler::handle_status_code(response, 200);
    response->set_body("Hello World! My payload includes a trailing CRLF.\r\n");

    std::string expected_result =
    {
        "HTTP/1.1 200 OK\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: 0\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "Hello World! My payload includes a trailing CRLF.\r\n",
    };
    ASSERT_TRUE(response->generate_response());
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

TEST(response_tests, content_types)
{
    Message::Response response;

    ASSERT_TRUE(response.set_content_type("/demo.txt"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/plain");

    ASSERT_TRUE(response.set_content_type("/demo.html"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/html");

    ASSERT_TRUE(response.set_content_type("/demo.css"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/css");

    ASSERT_TRUE(response.set_content_type("/demo.jpeg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    ASSERT_TRUE(response.set_content_type("/demo.jpg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    ASSERT_TRUE(response.set_content_type("/demo.png"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/png");

    ASSERT_TRUE(response.set_content_type("/demo.gif"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/gif");

    ASSERT_TRUE(response.set_content_type("/demo.svg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/svg+xml");

    ASSERT_TRUE(response.set_content_type("/demo.ico"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/x-icon");

    ASSERT_TRUE(response.set_content_type("/demo.json"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/json");

    ASSERT_TRUE(response.set_content_type("/demo.pdf"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/pdf");

    ASSERT_TRUE(response.set_content_type("/demo.js"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/javascript");

    ASSERT_TRUE(response.set_content_type("/demo.xml"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/xml");

    ASSERT_TRUE(response.set_content_type("/demo.wasm"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/wasm");

    ASSERT_TRUE(response.set_content_type("/demo.xhtml"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/xhtml+xml");

    // unsupported types
    ASSERT_FALSE(response.set_content_type("/demo.abc"));
    ASSERT_FALSE(response.set_content_type("/demo.dic"));
    ASSERT_FALSE(response.set_content_type("/demo.ext"));
    ASSERT_FALSE(response.set_content_type("/demo.xyz"));
    ASSERT_FALSE(response.set_content_type("/demo.com"));
};


TEST(response_tests, status_code_400_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();

    status_handler::handle_status_code(response, 400);

    ASSERT_TRUE(response->generate_response());
    
    std::string expected_response = 
    {
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 400 Bad Request :( </h1></html>\r\n"
    };
    ASSERT_EQ(response->get_response_message(), expected_response);
}

TEST(response_tests, status_code_404_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();

    status_handler::handle_status_code(response, 404);

    ASSERT_TRUE(response->generate_response());
    
    std::string expected_response = 
    {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 404 Not Found :( </h1></html>\r\n"
    };
    ASSERT_EQ(response->get_response_message(), expected_response);
}


TEST(response_tests, status_code_405_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();

    status_handler::handle_status_code(response, 405);

    ASSERT_TRUE(response->generate_response());
    
    std::string expected_response = 
    {
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Allow: GET, HEAD, PUT\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 405 Method Not Allowed :( </h1></html>\r\n"
    };
    ASSERT_EQ(response->get_response_message(), expected_response);
}

TEST(response_tests, status_code_500_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();

    status_handler::handle_status_code(response, 500);

    ASSERT_TRUE(response->generate_response());
    
    std::string expected_response = 
    {
        "HTTP/1.1 500 Internal Server Error\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><h1> 500 Internal Server Error :( </h1></html>\r\n"
    };
    ASSERT_EQ(response->get_response_message(), expected_response);
}

TEST(response_teset, read_local_file)
{
    Message::Response response;
    ASSERT_FALSE(response.read_file("/"));
}