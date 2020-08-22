#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "URI.hpp"

#include <memory>
#include <sstream>
#include <map>
#include <algorithm>

namespace Message
{
	class Request
	{
	public:
		~Request() noexcept;
		Request();

		Request& operator=(const Request&) ;
		Request(const Request&);
		
		Request(Request&&) noexcept;
		Request& operator=(Request&&) noexcept;

	public:
		/**
		 * @brief  Generate request message.
		 * @return  True if successfully generate request string.  
		 * 
		 * This generates request string based on request object's 
		 * members and set the generated string to private member
		 * @b impl_->raw_request.
		 */
		bool generate_request();

		/**
		 * @brief  Parse given request line(first line of request message).
		 * @param  request_line  Given request line in the form of string.
		 * @return  True if parse successfully.
		 */
		bool parse_request_line(const std::string& request_line);

		/** 
		 * @brief  Parse given headers string.
		 * @param  headers  headers string.
		 * @return  True if parse successfully.
		 */
		bool parse_headers(const std::string& headers);

		/**
		 * @brief  Parse raw request that has already set by set_raw_request()
		 * @return  True if parse raw request string successfully.
		 */
		bool parse_raw_request();

		/**
		 * @brief  Parse URI.
		 * @param  URI URI string.
		 * @return  True if successfully parse the given URI string.
		 */
		bool parse_uri(const std::string& URI);
		
		/**
		 * @brief  Set raw request message to request object's member variable.
		 * @param  raw_request_string  Given raw request message string.
		 * @return  True if successfully set raw request.
		 */
		bool set_raw_request(std::string raw_request_string);

		/**
		 * @brief  Set method field.
		 * @param  method  Method string. Default value is "GET".
		 * @return  True if successfully set method string.
		 */
		bool set_method(const std::string method = "GET");

		/**
		 * @brief  Set http version field.
		 * @param  http_version  Http version string. Default value is "HTTP/1.1"
		 * @return  True if successfully set http version string.
		 */
		bool set_http_version(const std::string http_version = "HTTP/1.1");

		/**
		 * @brief  Set User-Agent header field.
		 * @param  user_agent  User agent header field. Default value is "Bitate".
		 * @return  True if successfully set User Agent field.
		 */
		bool set_user_agent(std::string user_agent = "Bitate");

		/**
		 * @brief  Get request method string.
		 * @return Method string of request message.
		 */
		std::string get_request_method();

		/**
		 * @brief  Get request URI string.
		 * @return  Request URI string.
		 */
		std::string get_request_uri();

		/**
		 * @brief  Get request http protocol version.
		 * @return  Http protocol version string.
		 */
		std::string get_http_version();

		/**
		 * @brief  Get given request header.
		 * @return  Header name's corresponding header value.
		 */
		std::string get_header(const std::string& header_name);

		/**
		 * @brief  Get request body string.
		 * @return Request body string.
		 */
		std::string get_body();

		/**
		 * @brief  Get generated request string.
		 * @return  Request string.
		 */
		std::string get_generated_request();

		/**
		 * @brief  Has given header?
		 * @return  True if it has.
		 */
		bool has_header(const std::string& header_name) const;

		/**
		 * @brief  Has http version field set?
		 * @return  True if it sets.
		 */
		bool has_http_version() const;

		/**
		 * @brief  Has request method set? 
		 * @return True if it sets.
		 */
		bool has_method(const std::string& method) const;
		
		/**
		 * @brief  Get raw request.
		 * @return  Raw request string.
		 */
		std::string get_raw_request();

		/**
		 * @brief  Get host name.
		 * @return  Host string.
		 */
		std::string get_host();

		/**
		 * @brief  Get port string.
		 * @return  Port string.
		 */
		std::string get_port();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}

#endif