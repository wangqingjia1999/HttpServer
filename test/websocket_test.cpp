#include <gtest/gtest.h>

#include "websocket.hpp"
#include "Response.hpp"
#include "Request.hpp"

TEST(websocket_tests, parse_websocket_request_test)
{
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response > ();
    std::shared_ptr< Message::Request > request = std::make_shared< Message::Request > ();
	
	std::string raw_request = (
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: keep-alive\r\n"
		"Cache-Control: max-age=0\r\n"
		"\r\n"
	);
    ASSERT_TRUE(request->set_raw_request(raw_request));
    websocket websocket(request, response);

    ASSERT_FALSE(websocket.parse_websocket_request());
}

TEST(websocket_tests, generate_websocket_response_test)
{
	std::shared_ptr< Message::Response > response = std::make_shared< Message::Response > ();
    std::shared_ptr< Message::Request > request = std::make_shared< Message::Request > ();
	
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
    ASSERT_TRUE(request->set_raw_request(raw_request));
	ASSERT_TRUE(request->parse_raw_request());
	websocket websocket(request, response);

	ASSERT_TRUE(websocket.parse_websocket_request());

	std::string desired_response_string = 
	{
		"HTTP/1.1 101 Switching Protocol\r\n"
	};
	ASSERT_TRUE(websocket.generate_websocket_request());
	ASSERT_EQ(response->get_status_code(), 101);
	ASSERT_EQ(response->get_reason_phrase(), "Switching Protocol");
}

TEST(websoket_teset, generate_websocket_key_test)
{
	websocket websocket;

	struct test_vector
	{
		std::string raw_key;
		std::string expected_key;
	};

	const std::vector< test_vector > test_vectors = {
		{ "bKdPyn3u98cTfZJSh4TNeQ==", "4EaeSCkuOGBy+rjOSJSMV+VMoC0=" },
		{ "dGhlIHNhbXBsZSBub25jZQ==", "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=" }
	};

	size_t index = 0;
	for(auto test_vector : test_vectors)
	{
		ASSERT_EQ(websocket.generate_sec_websocket_key(test_vector.raw_key), test_vector.expected_key) << "Error at index: " << index;
		++index;
	}
}