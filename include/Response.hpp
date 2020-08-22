#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <memory>

#include "Request.hpp"

namespace Message
{
	class Response
	{
	public:
		Response(); 
		~Response() noexcept;
		
		Response(const Response&);
		Response& operator=(const Response&);
		
		Response(Response&&) noexcept;
		Response& operator=(Response&&) noexcept;

	public:
		bool has_header(const std::string& name);
		int get_status_code();
		std::string get_protocol_version();
		std::string get_reason_phrase();
		std::string get_body();
		std::string get_header(const std::string& header_name);
		std::string get_status_code_reason_string(const int status_code);
		std::string get_body_length();
		std::string get_response_message();
		size_t get_response_length();

		/**
		 * @brief  Set status code and corresponding reason phrase.
		 * @param  status_code  Response status code
		 * @return  True if set successfully.
		 */
		bool set_status(const int status_code);
		
		/**
		 * @brief  Set HTTP protocol version.
		 * @param  protocol_version  Protocol version.
		 * @return  True if set successfully.
		 */
		bool set_protocol_version(const std::string protocol_version);
		
		/**
		 * @brief  Set response body.
		 * @param  response_body  Response body string.
		 * @return  True if set successfully.
		 */
		bool set_body(const std::string& response_body);
		
		/**
		 * @brief  Set response body length.
		 * @param  body_length  Length of response body.
		 * @return  True if set successfully.
		 */
		bool set_body_length(const std::streamoff body_length);
		
		/**
		 * @brief  Add header.
		 * @param  name  Header name
		 * @param  value  Corresponding header value
		 * @return  True if add successfully.
		 */
		bool add_header(const std::string& name, const std::string& value);

		/**
		 * @brief  Set Content-Length header.
		 * @param  content_length  Length of response body.
		 * @return  True if set successfully.
		 */
		bool set_content_length(const std::streamoff& content_length);
		
		/**
		 * @brief  Determine content type based on the file extension.
		 * @param  request_uri  Request URI taken from request message
		 * @return  True if successfully set content type.
		 * 
		 * For all available content types, see:
		 * https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 */
		bool set_content_type(const std::string& request_uri);

		/**
		 * @brief  Map given URI to local file system.
		 * @param  request_uri  Request URI taken from request message
		 * @return  True if successfully set content.
		 */
		bool set_content(const std::string& request_uri);

		/**
		 * @brief  Set the complete response message.
		 * @param  response  Complete response message.
		 * @return  True if set successfully.
		 */
		bool set_response_message(const std::string& response);
		
		/**
		 * @brief  Set reason phrase.
		 * @param  status_code  Status code integer.
		 * @return  True if set reason phrase successfully.
		 */
		bool set_reason_phrase(const int stauts_code);

		/**
		 * @brief  Generate/Assemble response string.
		 */
		void generate_response();

		/**
		 * @brief  Read from local file system.
		 * @param  path  Path string.
		 * @return  True if read local file successfully.
		 * 
		 * Map the input path string to local file system 
		 * as the payloal/body of response message. 
		 * It will set the body and body length as well.
		 */
		bool read_file(const std::string& path);

		/**
		 * @brief  Convert given relative path to absolute path.
		 * @param  path  Relative path.
		 * @param  absolute_path  Generated absolute path.
		 * @return  True if convert successfully.
		 */
		bool convert_path_to_absolute(const std::string& relative_path, std::string& absolute_path);

		/**
		 * @brief  Clear up the header fields.
		 */
		void clear_up_header_fields();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}

#endif

