#include "Request.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(request_test, default_method_get_test)
{
	Message::Request request;
	request.set_method();
	ASSERT_EQ(request.get_request_method(), "GET");
}

TEST(request_test, default_http_version_test)
{
	Message::Request request;
	request.set_http_version();
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");
}

TEST(request_test, default_user_agent_test)
{
	Message::Request request;
	request.set_user_agent();
	ASSERT_EQ(request.get_header("User-Agent"), "Bitate");
}

TEST(request_test, generate_request_from_uri_test)
{
	Message::Request request;

	request.parse_uri("http://localhost:2333/");
	request.set_method();
	request.set_http_version();
	request.set_user_agent();

	std::string raw_request{"GET / HTTP/1.1\r\n"
	                        "Host: localhost\r\n"
	                        "User-Agent: Bitate\r\n"
	                        "\r\n"};

	ASSERT_TRUE(request.generate_request());
	ASSERT_EQ(request.get_generated_request(), raw_request);
}

TEST(request_test, parse_raw_get_request_test)
{
	Message::Request request;

	std::string raw_request{
	    "GET / HTTP/1.1\r\n"
	    "Host: localhost\r\n"
	    "Connection: keep-alive\r\n"
	    "Cache-Control: max-age=0\r\n"
	    "Upgrade-Insecure-Requests: 1\r\n"
	    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
	    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 "
	    "Safari/537.36\r\n"
	    "Accept: "
	    "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
	    "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
	    "Sec-Fetch-Site: none\r\n"
	    "Sec-Fetch-Mode: navigate\r\n"
	    "Sec-Fetch-User: ?1\r\n"
	    "Sec-Fetch-Dest: document\r\n"
	    "Accept-Encoding: gzip, deflate, br\r\n"
	    "Accept-Language: en,zh-CN;q=0.9,zh;q=0.8\r\n"
	    "\r\n"};
	request.set_raw_request(raw_request);
	ASSERT_TRUE(request.parse_raw_request());

	// test request line
	ASSERT_EQ(request.get_request_method(), "GET");
	ASSERT_EQ(request.get_request_uri()->get_path_string(), "/");
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");

	// test headers
	ASSERT_EQ(request.get_header("Host"), "localhost");
	ASSERT_EQ(request.get_header("Connection"), "keep-alive");
	ASSERT_EQ(request.get_header("Cache-Control"), "max-age=0");
	ASSERT_EQ(request.get_header("Upgrade-Insecure-Requests"), "1");
	ASSERT_EQ(request.get_header("User-Agent"),
	          "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
	          "(KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36");
	ASSERT_EQ(
	    request.get_header("Accept"),
	    "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
	    "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	ASSERT_EQ(request.get_header("Sec-Fetch-Site"), "none");
	ASSERT_EQ(request.get_header("Sec-Fetch-Mode"), "navigate");
	ASSERT_EQ(request.get_header("Sec-Fetch-User"), "?1");
	ASSERT_EQ(request.get_header("Sec-Fetch-Dest"), "document");
	ASSERT_EQ(request.get_header("Accept-Encoding"), "gzip, deflate, br");
	ASSERT_EQ(request.get_header("Accept-Language"), "en,zh-CN;q=0.9,zh;q=0.8");

	// test body
	ASSERT_EQ(request.get_body(), "");
}

TEST(request_tests, parse_post_request_test)
{
	Message::Request request;

	std::string raw_post_request_message{
	    "POST / HTTP/1.1\r\n"
	    "Host: 192.168.72.128:2333\r\n"
	    "Connection: keep-alive\r\n"
	    "Content-Length: 61\r\n"
	    "Cache-Control: max-age=0\r\n"
	    "Upgrade-Insecure-Requests: 1\r\n"
	    "Origin: http://192.168.72.128:2333\r\n"
	    "Content-Type: application/x-www-form-urlencoded\r\n"
	    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
	    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 "
	    "Safari/537.36\r\n"
	    "Accept: "
	    "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
	    "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
	    "Referer: http://192.168.72.128:2333/\r\n"
	    "Accept-Encoding: gzip, deflate\r\n"
	    "Accept-Language: en,zh-CN;q=0.9,zh;q=0.8\r\n"
	    "\r\n"
	    "Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer"};

	request.set_raw_request(raw_post_request_message);
	ASSERT_TRUE(request.parse_raw_request());

	// test request line
	ASSERT_EQ(request.get_request_method(), "POST");
	ASSERT_EQ(request.get_request_uri()->get_path_string(), "/");
	ASSERT_EQ(request.get_http_version(), "HTTP/1.1");

	// test headers
	ASSERT_EQ(request.get_header("Host"), "192.168.72.128:2333");
	ASSERT_EQ(request.get_header("Connection"), "keep-alive");
	ASSERT_EQ(request.get_header("Content-Length"), "61");
	ASSERT_EQ(request.get_header("Cache-Control"), "max-age=0");
	ASSERT_EQ(request.get_header("Upgrade-Insecure-Requests"), "1");
	ASSERT_EQ(request.get_header("Origin"), "http://192.168.72.128:2333");
	ASSERT_EQ(request.get_header("Content-Type"),
	          "application/x-www-form-urlencoded");
	ASSERT_EQ(request.get_header("Referer"), "http://192.168.72.128:2333/");
	ASSERT_EQ(request.get_header("User-Agent"),
	          "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
	          "(KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36");
	ASSERT_EQ(
	    request.get_header("Accept"),
	    "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
	    "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	ASSERT_EQ(request.get_header("Accept-Encoding"), "gzip, deflate");
	ASSERT_EQ(request.get_header("Accept-Language"), "en,zh-CN;q=0.9,zh;q=0.8");

	// test body
	ASSERT_EQ(request.get_body(),
	          "Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer");
}

TEST(request_tests, whether_request_contains_query_string_test)
{
	Message::Request request;

	std::string raw_request{"GET /search?q=test HTTP/1.1\r\n"
	                        "Host: www.bitate.com\r\n"
	                        "\r\n"};

	request.set_raw_request(raw_request);
	ASSERT_TRUE(request.parse_raw_request());

	ASSERT_TRUE(request.has_query());
	ASSERT_EQ(request.get_request_uri()->get_query(), "q=test");
}

TEST(request_tests, case_insensitive_headers_test)
{
	Message::Request request;

	std::string raw_request{"GET / HTTP/1.1\r\n"
	                        "HoSt: www.bitate.com\r\n"
	                        "ConNecTion: keep-alive\r\n"
	                        "CONTENT-Length: 61\r\n"
	                        "\r\n"};

	request.set_raw_request(raw_request);
	ASSERT_TRUE(request.parse_raw_request());

	ASSERT_EQ(request.get_header("Host"), "www.bitate.com");
	ASSERT_EQ(request.get_header("Connection"), "keep-alive");
	ASSERT_EQ(request.get_header("Content-Length"), "61");
}