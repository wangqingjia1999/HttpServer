#include <gtest/gtest.h>
#include "Request.hpp"

TEST(RequestTests, parseRawGetRequest)
{
	Message::Request request;

	std::string rawRequest = (
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: keep-alive\r\n"
		"Cache-Control: max-age=0\r\n"
		"Upgrade-Insecure-Requests: 1\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
		"Sec-Fetch-Site: none\r\n"
		"Sec-Fetch-Mode: navigate\r\n"
		"Sec-Fetch-User: ?1\r\n"
		"Sec-Fetch-Dest: document\r\n"
		"Accept-Encoding: gzip, deflate, br\r\n"
		"Accept-Language: en,zh-CN;q=0.9,zh;q=0.8\r\n"
		"\r\n"
		);
	ASSERT_TRUE(request.setRawRequest(rawRequest));
	ASSERT_TRUE(request.parseRawRequest());

	// test request line
	ASSERT_EQ(request.getMethod(), "GET");
	ASSERT_EQ(request.getRequestUri(), "/");
	ASSERT_EQ(request.getHttpVersion(), "HTTP/1.1");
	
	// test headers
	ASSERT_EQ(request.getHeader("Host"), "localhost");
	ASSERT_EQ(request.getHeader("Connection"), "keep-alive");
	ASSERT_EQ(request.getHeader("Cache-Control"), "max-age=0");
	ASSERT_EQ(request.getHeader("Upgrade-Insecure-Requests"), "1");
	ASSERT_EQ(request.getHeader("User-Agent"), "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36");
	ASSERT_EQ(request.getHeader("Accept"), "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	ASSERT_EQ(request.getHeader("Sec-Fetch-Site"), "none");
	ASSERT_EQ(request.getHeader("Sec-Fetch-Mode"), "navigate");
	ASSERT_EQ(request.getHeader("Sec-Fetch-User"), "?1");
	ASSERT_EQ(request.getHeader("Sec-Fetch-Dest"), "document");
	ASSERT_EQ(request.getHeader("Accept-Encoding"), "gzip, deflate, br");
	ASSERT_EQ(request.getHeader("Accept-Language"), "en,zh-CN;q=0.9,zh;q=0.8");

	// test body
	ASSERT_EQ(request.getBody(), "");
}

