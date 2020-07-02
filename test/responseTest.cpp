#include <gtest/gtest.h>
#include "Response.hpp"

TEST(ResponseTests, addHeaders) {
    Message::Response response;

    ASSERT_EQ(response.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT"), true);
    ASSERT_EQ(response.addHeader("Accept-Ranges", "bytes"), true);
    ASSERT_EQ(response.addHeader("Content-Type", "text/plain"), true);
    ASSERT_EQ(response.get_header("Date"), "Mon, 27 Jul 2009 12:28:53 GMT");
    ASSERT_EQ(response.get_header("Accept-Ranges"), "bytes");
    ASSERT_EQ(response.get_header("Content-Type"), "text/plain");
}

TEST(ResponseTests, generateGetResponse)
{
    Message::Response response;

    response.setStatus(200);
    response.setBody("Hello World! My payload includes a trailing CRLF.\r\n");

    ASSERT_EQ(response.addHeader("Accept-Ranges", "bytes"), true);
    ASSERT_EQ(response.addHeader("Content-Length", "%zu"), true);
    ASSERT_EQ(response.addHeader("Content-Type", "text/plain"), true);
    ASSERT_EQ(response.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT"), true);

    std::string responseResult =
    {
        "HTTP/1.1 200 OK\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: %zu\r\n"
        "Content-Type: text/plain\r\n"
        "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
        "\r\n"
        "Hello World! My payload includes a trailing CRLF.\r\n",
    };
    ASSERT_TRUE(response.generate_response());
    ASSERT_EQ(response.get_response_message(), responseResult);
}

TEST(ResponseTests, mapStatusCodeToReasonString)
{
    Message::Response response;

    ASSERT_EQ(response.getStatusCodeReasonString(100), "Continue");
    ASSERT_EQ(response.getStatusCodeReasonString(200), "OK");
    ASSERT_EQ(response.getStatusCodeReasonString(307), "Temporary Redirect");
    ASSERT_EQ(response.getStatusCodeReasonString(400), "Bad Request");
    ASSERT_EQ(response.getStatusCodeReasonString(404), "Not Found");
    ASSERT_EQ(response.getStatusCodeReasonString(500), "Internal Server Error");
    ASSERT_EQ(response.getStatusCodeReasonString(502), "Bad Gateway");
    ASSERT_EQ(response.getStatusCodeReasonString(505), "HTTP Version Not Supported");
}

TEST(ResponseTests, ResponseContentTypeTest)
{
    Message::Response response;

    ASSERT_TRUE(response.setContentType("/demo.txt"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/plain");

    ASSERT_TRUE(response.setContentType("/demo.html"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/html");

    ASSERT_TRUE(response.setContentType("/demo.css"));
    ASSERT_EQ(response.get_header("Content-Type"), "text/css");

    ASSERT_TRUE(response.setContentType("/demo.jpeg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    ASSERT_TRUE(response.setContentType("/demo.jpg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/jpg");

    ASSERT_TRUE(response.setContentType("/demo.png"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/png");

    ASSERT_TRUE(response.setContentType("/demo.gif"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/gif");

    ASSERT_TRUE(response.setContentType("/demo.svg"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/svg+xml");

    ASSERT_TRUE(response.setContentType("/demo.ico"));
    ASSERT_EQ(response.get_header("Content-Type"), "image/x-icon");

    ASSERT_TRUE(response.setContentType("/demo.json"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/json");

    ASSERT_TRUE(response.setContentType("/demo.pdf"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/pdf");

    ASSERT_TRUE(response.setContentType("/demo.js"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/javascript");

    ASSERT_TRUE(response.setContentType("/demo.xml"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/xml");

    ASSERT_TRUE(response.setContentType("/demo.wasm"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/wasm");

    ASSERT_TRUE(response.setContentType("/demo.xhtml"));
    ASSERT_EQ(response.get_header("Content-Type"), "application/xhtml+xml");

    // unsupported types
    ASSERT_FALSE(response.setContentType("/demo.abc"));
    ASSERT_FALSE(response.setContentType("/demo.dic"));
    ASSERT_FALSE(response.setContentType("/demo.ext"));
    ASSERT_FALSE(response.setContentType("/demo.xyz"));
    ASSERT_FALSE(response.setContentType("/demo.com"));
};


