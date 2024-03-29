#pragma once

#include "Logger.hpp"
#include "Uri.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

namespace Message
{
	struct CaseInsensitiveComparator
	{
		bool operator()(const std::string& lhs, const std::string& rhs) const
		{
			return ::strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
		}
	};

	class Request
	{
	public:
		~Request() = default;
		Request() = default;

		Request& operator=(const Request& other) = default;
		Request(const Request& other) = default;

		Request(Request&&) noexcept = default;
		Request& operator=(Request&&) noexcept = default;

		/**
		 * Generate request message.
		 *
		 * @return
		 * 		True if successfully generate request string.
		 *
		 * @note
		 * 		This generates request string based on request object's
		 * 		members and set the generated string to private member
		 * 		@b raw_request.
		 */
		bool generate_request();

		/**
		 * Parse given request line(first line of request message).
		 *
		 * @param[in] request_line
		 * 		Given request line in the form of string.
		 *
		 * @return
		 * 		True if parse successfully.
		 */
		bool parse_request_line(const std::string& request_line);

		/**
		 * Parse given headers string.
		 *
		 * @param new_headers
		 * 		headers string.
		 *
		 * @return
		 * 		True if parse successfully.
		 */
		bool parse_headers(const std::string& new_headers);

		/**
		 * Parse raw request that has already set by set_raw_request()
		 *
		 * @return
		 * 		True if parse raw request string successfully.
		 */
		bool parse_raw_request();

		/**
		 * Parse Uri and fill in headers in the request.
		 *
		 * @param[in] Uri
		 * 		Uri string.
		 *
		 * @return
		 * 		True if successfully parse the given Uri string.
		 */
		bool parse_uri(const std::string& Uri);

		bool has_header(const std::string& header_name) const;
		bool has_query() const;

		void set_raw_request(std::string raw_request_string);
		void set_method(std::string new_method = "GET");
		void set_http_version(std::string new_http_version = "HTTP/1.1");
		void set_user_agent(std::string new_user_agent = "Bitate");

		std::string get_raw_request();
		std::string get_request_method();
		std::shared_ptr<Uri> get_request_uri();
		std::string get_request_uri_string();
		std::string get_http_version();
		std::string get_header(const std::string& header_name);
		std::string get_body();
		std::string get_generated_request();
		std::string get_host();
		std::string get_port();

		/**
		 * Clear up all the fields of a request object.
		 */
		void clear_up();

	private:
		/**
		 * For search bar: /?Search=This+is+just+a+demo
		 * For normal get: /index.html
		 * For more info, see: https://tools.ietf.org/html/rfc2616#section-5.1.2
		 */
		std::shared_ptr<Uri> m_uri = std::make_shared<Uri>();

		// Generated/Received raw request string
		std::string m_raw_request;

		// Request method. e.g. GET, POST
		std::string m_method;

		// Request uri string
		std::string m_request_uri;

		// Default http_version
		std::string m_http_version = "HTTP/1.1";

		// Store the generated/received raw headers
		std::string m_headers;

		using CaseInsensitiveMap =
		    std::map<std::string, std::string, CaseInsensitiveComparator>;

		// Contain the header {key:value} pairs
		CaseInsensitiveMap m_headers_map;

		// Request body string
		std::string m_body;
	};
} // namespace Message
