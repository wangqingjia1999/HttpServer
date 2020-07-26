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

    ASSERT_TRUE(websocket.parse_websocket_request());
}

TEST(websocket_tests, generate_websocket_response_test)
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

	ASSERT_TRUE(websocket.parse_websocket_request());

	std::string desired_response_string = 
	{
		"HTTP/1.1 101 Switching Protocol\r\n"
	};
	ASSERT_TRUE(websocket.generate_websocket_request());
	ASSERT_EQ(response->get_status_code(), 101);
	ASSERT_EQ(response->get_reason_phrase(), "Switching Protocol");
}