#include "Response.hpp"
#include "StatusHandler.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace
{
	/**
	 * Weed out dynamically generated http date to facilitate the checking of
	 * two http messages.
	 *
	 * @param[in] message
	 *      Http request/response message.
	 *
	 * @return
	 *      Http request/response message without date header.
	 */
	inline std::string weed_out_http_date_header(const std::string& message)
	{
		// Weed out: "Date: Thu, 12 Nov 2020 13:41:37 GMT\r\n"
		if (message.find("Date:") != std::string::npos &&
		    message.find("GMT") != std::string::npos)
		{
			std::string before = message.substr(0, message.find("Date:"));
			std::string after = message.substr(message.find("GMT") + 5);
			return before + after;
		}
		return message;
	}
} // namespace

TEST(response_tests, add_headers)
{
	Message::Response response;

	ASSERT_TRUE(response.add_header("Date", "Mon, 27 Jul 2009 12:28:53 GMT"));
	ASSERT_TRUE(response.add_header("Accept-Ranges", "bytes"));
	ASSERT_TRUE(response.add_header("Content-Type", "text/plain"));
	ASSERT_EQ(response.get_header("Date"), "Mon, 27 Jul 2009 12:28:53 GMT");
	ASSERT_EQ(response.get_header("Accept-Ranges"), "bytes");
	ASSERT_EQ(response.get_header("Content-Type"), "text/plain");
}

TEST(response_tests, generate_200_response)
{
	std::shared_ptr<Message::Response> response =
	    std::make_shared<Message::Response>();

	std::string body = "Hello World! My payload includes a trailing CRLF.\r\n";
	response->set_body(body);
	response->set_content_type("text/html");
	StatusHandler::handle_status_code(response, 200);

	std::string expected_result{
	    "HTTP/1.1 200 OK\r\n"
	    "Content-Length: 51\r\n"
	    "Content-Type: text/html\r\n"
	    "Host: www.bitate.com\r\n"
	    "Server: Bitate\r\n"
	    "\r\n"
	    "Hello World! My payload includes a trailing CRLF.\r\n",
	};

	EXPECT_EQ(weed_out_http_date_header(response->generate_response()),
	          expected_result);
}

TEST(response_tests, map_status_code_to_reaseon_phrase)
{
	Message::Response response;

	ASSERT_EQ(response.get_status_code_reason_string(100), "Continue");
	ASSERT_EQ(response.get_status_code_reason_string(200), "OK");
	ASSERT_EQ(response.get_status_code_reason_string(307),
	          "Temporary Redirect");
	ASSERT_EQ(response.get_status_code_reason_string(400), "Bad Request");
	ASSERT_EQ(response.get_status_code_reason_string(404), "Not Found");
	ASSERT_EQ(response.get_status_code_reason_string(500),
	          "Internal Server Error");
	ASSERT_EQ(response.get_status_code_reason_string(502), "Bad Gateway");
	ASSERT_EQ(response.get_status_code_reason_string(505),
	          "HTTP Version Not Supported");
}

TEST(response_tests, response_header_serialization_test)
{
	Message::Response response;

	ASSERT_TRUE(response.add_header("Date", "Mon, 27 Jul 2009 12:28:53 GMT"));
	ASSERT_TRUE(response.add_header("Accept-Ranges", "bytes"));
	ASSERT_TRUE(response.add_header("Content-Length", "51"));
	ASSERT_TRUE(response.add_header("Content-Type", "text/plain"));

	std::string expected_result{"Accept-Ranges:bytes\n"
	                            "Content-Length:51\n"
	                            "Content-Type:text/plain\n"
	                            "\n\n\n\n"};

	ASSERT_EQ(response.serialize_headers(), expected_result);
}

TEST(response_tests, response_header_serialization_empty_header_test)
{
	Message::Response response;

	std::string expected_result{"\n\n\n\n"};

	ASSERT_EQ(response.serialize_headers(), expected_result);
}

TEST(response_tests, response_header_deserialization_test)
{
	Message::Response response;

	std::string serialzied_message{"Accept-Ranges:bytes\n"
	                               "Content-Length:51\n"
	                               "Content-Type:text/plain\n"
	                               "\n\n\n\n"
	                               "<html>this is body of response.</html>"};

	response.deserialize(serialzied_message);

	ASSERT_EQ(response.get_header("Content-Length"), "38");
	ASSERT_EQ(response.get_header("Accept-Ranges"), "bytes");
	ASSERT_EQ(response.get_header("Content-Type"), "text/plain");
	ASSERT_EQ(response.get_body(), "<html>this is body of response.</html>");
}