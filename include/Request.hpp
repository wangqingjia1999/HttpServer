#pragma once

#include "Uri.hpp"
#include <memory>
#include <sstream>
#include <map>
#include <algorithm>

namespace Message
{
	class Request
	{
	public:
		~Request();
		Request();
		Request(const Request&) noexcept;
		Request(Request&&) noexcept;
		Request& operator=(const Request&) noexcept;
		Request& operator=(Request&&) noexcept;

	public:
		/**
		 * @brief
		 * 		Generate request message.
		 * @return 
		 * 		true/false.
		 */
		bool generate_request();

		/**
		 * @brief
		 * 		Parse given request line(first line of request message).
		 *
		 * @param[in] 
		 *		request_line is the given request line.
		 *
		 * @return
		 *		true/false.
		 */
		bool parse_request_line(const std::string& request_line);

		/** 
		 * @brief
		 * 		Parse given headers.
		 * @param[in] 
		 *		headers is the map that stores name-value
		 *		header pairs.
		 * @return 
		 *		true/false.
		 */
		bool parse_headers(const std::string& headers);

		/**
		 * @brief
		 * 		Parse raw request that has already set by set_raw_request()
		 * @return 
		 *		true/false.
		 */
		bool parse_raw_request();

		/**
		 * @brief
		 * 		Parse Uri.
		 * @param[in]
		 * 		Uri string.
		 * @return
		 * 		true/false.
		 */
		bool parse_uri(const std::string& Uri);
		
		/**
		 * @brief
		 * 		Set received raw request message to request object's member variable.
		 * @param rawRequestString
		 * 		Received raw request message string.
		 * @param raw_request
		 * 		Object request's member raw_request is set.
		 * @return 
		 * 		true/false
		 */
		bool set_raw_request(std::string rawRequestString);

		/**
		 * @brief
		 * 		Set method field.
		 * @param[in] 
		 * 		A method string. Default value is "GET".
		 * @return
		 * 		true/false.
		 */
		bool set_method(const std::string method = "GET");

		/**
		 * @brief
		 * 		Set http version field.
		 * @param[in]
		 * 		A http version string. Default value is "HTTP/1.1"
		 * @return
		 * 		true/false.
		 */
		bool set_http_version(const std::string http_version = "HTTP/1.1");

		/**
		 * @brief
		 * 		Set User-Agent header field.
		 * @param[in]
		 * 		An User-Agent string with default value of "Bitate".
		 * @return 
		 * 		true/false.
		 */
		bool set_user_agent(std::string user_agent = "Bitate");

		std::string get_method();
		std::string get_request_uri();
		std::string get_http_version();
		std::string get_header(const std::string& header_name);
		std::string get_body();
		std::string get_generated_request();

		bool has_header(const std::string& header_name) const;
		bool has_http_version() const;
		bool has_method(const std::string& method) const;
		
		/**
		 * @brief
		 * 		Get raw request.
		 * @return 
		 * 		Raw request string.
		 */
		std::string get_raw_request();

		/**
		 * @brief 
		 * 		Get host name.
		 * @return
		 * 		Host string.
		 */
		std::string get_host();

		/**
		 * @brief
		 * 		Get port string.
		 * @return
		 * 		Port string.
		 */
		std::string get_port();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}


