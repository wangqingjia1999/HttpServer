#pragma once

#include "Request.hpp"

#include <map>
#include <set>
#include <regex>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

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
		 * Whether response has header with name of @name.
		 * 
		 * @param[in] name  
		 * 		Header name.
		 * 
		 * @return  
		 * 		True if has header with name of @name.
		 */
		bool has_header(const std::string& name);

		/**
		 * Get response's status code.
		 * 
		 * @return  
		 * 		An integer represents status code.
		 */
		int get_status_code();

		/**
		 * Get response's protocol version.
		 * 
		 * @return  
		 * 		Protocol version. Default value is "HTTP/1.1".
		 */
		std::string get_protocol_version();

		/**
		 * Get response's reason phrase.
		 * 	
		 * @return 
		 * 		Reason phrase.
		 */
		std::string get_reason_phrase();

		/**
		 * Get response's body.
		 * 
		 * @return  
		 * 		Response's body string.
		 */
		std::string get_body();

		/**
		 * Get response's specific header value of @header_name.
		 * 
		 * @param[in] header_name  
		 * 		Response header name. 
		 * 
		 * @return  
		 * 		Corresponding header value.
		 */
		std::string get_header(const std::string& header_name);

		/**
		 * Get status code's corresponding reason phrase.
		 * 
		 * @param[in] status_code  
		 * 		Status code integer.
		 * 
		 * @return  
		 * 		Corresponding reason phrase of given status_code.
		 */
		std::string get_status_code_reason_string(const int status_code);

		/**
		 * Get response's body length
		 * 
		 * @return  
		 * 		Response body length.
		 */
		std::string get_body_length();

		/**
		 * Get generated response message string.
		 * 
		 * @return  
		 * 		Response string.
		 */
		std::string get_response_message();

		/**
		 * Get response message length.
		 * 
		 * @return  
		 * 		Size of response message.
		 */
		size_t get_response_length();

		/**
		 * Get content type.
		 * 
		 * @return  
		 * 		Content type.
		 */
		std::string get_content_type();

		/**
		 * Set status code and corresponding reason phrase.
		 * 
		 * @param[in] status_code  
		 * 		Response status code
		 * 
		 * @return  
		 * 		True if set successfully.
		 */
		bool set_status(const int new_status_code);
		
		/**
		 * Set HTTP protocol version.
		 * 
		 * @param[in] protocol_version  
		 * 		Protocol version.
		 * 
		 * @return  
		 * 		True if set successfully.
		 */
		bool set_protocol_version(const std::string new_protocol_version);
		
		/**
		 * Set response body.
		 * 
		 * @param[in]  response_body  
		 * 		Response body string.
		 * 
		 * @return  
		 * 		True if set successfully.
		 */
		bool set_body(const std::string& new_response_body);
		
		/**
		 * Set response body length.
		 * 	
		 * @param body_length  
		 * 		Length of response body.
		 * 
		 * @return 
		 * 		True if set successfully.
		 */
		bool set_body_length(const std::streamoff new_body_length);
		
		/**
		 * Add header.
		 * 	
		 * @param[in] name  
		 * 		Header name
		 * 
		 * @param[in] value  
		 * 		Corresponding header value
		 * 
		 * @return  
		 * 		True if add successfully.
		 */
		bool add_header(const std::string& name, const std::string& value);

		/**
		 * Set Content-Length header.
		 * 
		 * @param[in] content_length  
		 * 		Length of response body.
		 * 
		 * @return  
		 * 		True if set successfully.
		 */
		bool set_content_length(const size_t& new_content_length);
		
		/**
		 * Set content type.
		 * 
		 * @param[in] content_type  
		 * 		Content type.
		 */
		void set_content_type(const std::string& new_content_type);

		/**
		 * Map given Uri to local file system.
		 * 
		 * @param[in] request_uri  
		 * 		Request Uri taken from request message
		 * 
		 * @return  
		 * 		True if successfully set content.
		 */
		bool set_content(const std::string& new_request_uri);

		/**
		 * Set the complete response message.
		 * 
		 * @param[in] response 
		 * 		Complete response message.
		 * 
		 * @return  
		 * 		True if set successfully.
		 */
		bool set_response_message(const std::string& new_response_message_string);
		
		/**
		 * Set reason phrase.
		 * 
		 * @param[in] status_code  
		 * 		Status code integer.
		 * 
		 * @return  
		 * 		True if set reason phrase successfully.
		 */
		bool set_reason_phrase(const int new_stauts_code);

		/**
		 * Assemble response string from member variables.
		 */
		void generate_response();

		/**
		 * Read from local file system.
		 * 
		 * @param[in] path  
		 * 		Path string.
		 * 
		 * @return  
		 * 		True if read local file successfully.
		 * 
		 * @note 
		 * 		Map the input path string to local file system 
		 * 		as the payloal/body of response message. 
		 * 		It will set the body and body length as well.
		 */
		bool read_file(const std::string& path);

		/**
		 * Determine content type based on the file extension.
		 * 
		 * @param[in] request_uri  
		 * 		Request Uri taken from request message
		 * 
		 * @note 
		 * 		For all available content types, see:
		 * 		https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 */
		void parse_content_type(const std::string& request_uri);

		/**
		 * Convert given relative path to absolute path.
		 * 
		 * @param[in] path 
		 * 		Relative path.
		 * 
		 * @param[in] absolute_path  
		 * 		Generated absolute path.
		 * 
		 * @return  
		 * 		True if convert successfully.
		 */
		bool convert_path_to_absolute(const std::string& relative_path, std::string& absolute_path);

		/**
		 * Clear up the header fields.
		 */
		void clear_up_header_fields();

	private:
		std::shared_ptr< Uri > uri;
		
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
