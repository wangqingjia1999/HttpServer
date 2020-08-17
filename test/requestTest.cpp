#include <gtest/gtest.h>
#include "Request.hpp"

TEST(RequestTests, defaultMethodGET)
{
	Message::Request request;
	ASSERT_TRUE(request.set_method());
	ASSERT_EQ(request.get_request_method(), "GET");
}

TEST(RequestTests, defaultHttpVersion)
{
	Message::Request request;
	ASSERT_TRUE(request.set_http_version());
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");
}

TEST(RequestTests, defaultUserAgent)
{
	Message::Request request;
	ASSERT_TRUE(request.set_user_agent());
	ASSERT_EQ(request.get_header("User-Agent"), "Bitate");
}

TEST(RequestTests, generateRequestFromUri)
{
	Message::Request request;
	
	ASSERT_TRUE(request.parse_uri("http://localhost:2333/"));
	ASSERT_TRUE(request.set_method());
	ASSERT_TRUE(request.set_http_version());
	ASSERT_TRUE(request.set_user_agent());

	std::string raw_request = 
	{
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"User-Agent: Bitate\r\n"
		"\r\n"
	};

	ASSERT_TRUE(request.generate_request());
	ASSERT_EQ(request.get_generated_request(), raw_request);
}

TEST(RequestTests, parseRawGetRequest)
{
	Message::Request request;

	std::string raw_request = (
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
	ASSERT_TRUE(request.set_raw_request(raw_request));
	ASSERT_TRUE(request.parse_raw_request());

	// test request line
	ASSERT_EQ(request.get_request_method(), "GET");
	ASSERT_EQ(request.get_request_uri(), "/");
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");
	
	// test headers
	ASSERT_EQ(request.get_header("Host"), "localhost");
	ASSERT_EQ(request.get_header("Connection"), "keep-alive");
	ASSERT_EQ(request.get_header("Cache-Control"), "max-age=0");
	ASSERT_EQ(request.get_header("Upgrade-Insecure-Requests"), "1");
	ASSERT_EQ(request.get_header("User-Agent"), "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36");
	ASSERT_EQ(request.get_header("Accept"), "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	ASSERT_EQ(request.get_header("Sec-Fetch-Site"), "none");
	ASSERT_EQ(request.get_header("Sec-Fetch-Mode"), "navigate");
	ASSERT_EQ(request.get_header("Sec-Fetch-User"), "?1");
	ASSERT_EQ(request.get_header("Sec-Fetch-Dest"), "document");
	ASSERT_EQ(request.get_header("Accept-Encoding"), "gzip, deflate, br");
	ASSERT_EQ(request.get_header("Accept-Language"), "en,zh-CN;q=0.9,zh;q=0.8");

	// test body
	ASSERT_EQ(request.get_body(), "");
}

TEST(request_tests, post_request_test)
{
	Message::Request request;

	std::string raw_post_request_message = {
		"POST / HTTP/1.1\r\n"
		"Host: 192.168.72.128:2333\r\n"
		"Connection: keep-alive\r\n"
		"Content-Length: 61\r\n"
		"Cache-Control: max-age=0\r\n"
		"Upgrade-Insecure-Requests: 1\r\n"
		"Origin: http://192.168.72.128:2333\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
		"Referer: http://192.168.72.128:2333/\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Accept-Language: en,zh-CN;q=0.9,zh;q=0.8\r\n"
		"\r\n"
		"Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer"
	};

	ASSERT_TRUE(request.set_raw_request(raw_post_request_message));
	ASSERT_TRUE(request.parse_raw_request());

	// test request line
	ASSERT_EQ(request.get_request_method(), "POST");
	ASSERT_EQ(request.get_request_uri(), "/");
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");
	
	// test headers
	ASSERT_EQ(request.get_header("Host"), "192.168.72.128:2333");
	ASSERT_EQ(request.get_header("Connection"), "keep-alive");
	ASSERT_EQ(request.get_header("Content-Length"), "61");
	ASSERT_EQ(request.get_header("Cache-Control"), "max-age=0");
	ASSERT_EQ(request.get_header("Upgrade-Insecure-Requests"), "1");
	ASSERT_EQ(request.get_header("Origin"), "http://192.168.72.128:2333");
	ASSERT_EQ(request.get_header("Content-Type"), "application/x-www-form-urlencoded");
	ASSERT_EQ(request.get_header("Referer"), "http://192.168.72.128:2333/");
	ASSERT_EQ(request.get_header("User-Agent"), "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36");
	ASSERT_EQ(request.get_header("Accept"), "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	ASSERT_EQ(request.get_header("Accept-Encoding"), "gzip, deflate");
	ASSERT_EQ(request.get_header("Accept-Language"), "en,zh-CN;q=0.9,zh;q=0.8");	

	// test body
	ASSERT_EQ(request.get_body(), "Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer");
}