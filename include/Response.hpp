#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <memory>

#include <unistd.h>

namespace Message
{
	class Response
	{
	public:
		Response(); 
		~Response() noexcept;
		
		Response(const Response&);
		Response& operator=(const Response&);
		
		Response(Response&&) noexcept = delete;
		Response& operator=(Response&&) noexcept = delete;

	public:
		/**
		 * @brief  Whether response has header with name of @name.
		 * @param  name  Header name.
		 * @return  True if has header with name of @name.
		 */
		bool has_header(const std::string& name);

		/**
		 * @brief  Get response's status code.
		 * @return  An integer represents status code.
		 */
		int get_status_code();

		/**
		 * @brief  Get response's protocol version.
		 * @return  Protocol version. Default value is "HTTP/1.1".
		 */
		std::string get_protocol_version();

		/**
		 * @brief  Get response's reason phrase.
		 * @return Reason phrase.
		 */
		std::string get_reason_phrase();

		/**
		 * @brief  Get response's body.
		 * @return  Response's body string.
		 */
		std::string get_body();

		/**
		 * @brief  Get response's specific header value of @header_name.
		 * @param  header_name  Response header name. 
		 * @return  Corresponding header value.
		 */
		std::string get_header(const std::string& header_name);

		/**
		 * @brief  Get status code's corresponding reason phrase.
		 * @param  status_code  Status code integer.
		 * @return  Corresponding reason phrase of given status_code.
		 */
		std::string get_status_code_reason_string(const int status_code);

		/**
		 * @brief  Get response's body length
		 * @return  Response body length.
		 */
		std::string get_body_length();

		/**
		 * @brief  Get generated response message string.
		 * @return  Response string.
		 */
		std::string get_response_message();

		/**
		 * @brief  Get response message length.
		 * @return  Size of response message.
		 */
		size_t get_response_length();

		/**
		 * @brief  Get content type.
		 * @return  Content type.
		 */
		std::string get_content_type();

		/**
		 * @brief  Set status code and corresponding reason phrase.
		 * @param  status_code  Response status code
		 * @return  True if set successfully.
		 */
		bool set_status(const int new_status_code);
		
		/**
		 * @brief  Set HTTP protocol version.
		 * @param  protocol_version  Protocol version.
		 * @return  True if set successfully.
		 */
		bool set_protocol_version(const std::string new_protocol_version);
		
		/**
		 * @brief  Set response body.
		 * @param  response_body  Response body string.
		 * @return  True if set successfully.
		 */
		bool set_body(const std::string& new_response_body);
		
		/**
		 * @brief  Set response body length.
		 * @param  body_length  Length of response body.
		 * @return  True if set successfully.
		 */
		bool set_body_length(const std::streamoff new_body_length);
		
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
		bool set_content_length(const size_t& new_content_length);
		
		/**
		 * @brief  Set content type.
		 * @param  content_type  Content type.
		 */
		void set_content_type(const std::string& new_content_type);

		/**
		 * @brief  Map given URI to local file system.
		 * @param  request_uri  Request URI taken from request message
		 * @return  True if successfully set content.
		 */
		bool set_content(const std::string& new_request_uri);

		/**
		 * @brief  Set the complete response message.
		 * @param  response  Complete response message.
		 * @return  True if set successfully.
		 */
		bool set_response_message(const std::string& new_response_message_string);
		
		/**
		 * @brief  Set reason phrase.
		 * @param  status_code  Status code integer.
		 * @return  True if set reason phrase successfully.
		 */
		bool set_reason_phrase(const int new_stauts_code);

		/**
		 * @brief  Assemble response string from member variables.
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
		 * @brief  Determine content type based on the file extension.
		 * @param  request_uri  Request URI taken from request message
		 * 
		 * For all available content types, see:
		 * https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 */
		void parse_content_type(const std::string& request_uri);

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
		std::shared_ptr< URI > uri;
		
		// Status code of response
		int status_code;

		// Default protocol version
		std::string protocol_version;
		
		// Reason phrase for specific status code.
		std::string reason_phrase;
		
		// Header fields
		std::map < std::string, std::string > headers;
		
		// Body of response message
		std::string body;
		
		// Length of body of response message
		std::streamoff body_length;

		// Content type
		std::string content_type;

		size_t content_length;

		// Generated response message string
		std::string response_message;
	};
}

#endif

