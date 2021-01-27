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
		 * 		True if succeeds.
		 */
		bool set_status(const int status_code);
		
		/**
		 * Set HTTP protocol version.
		 * 
		 * @param[in] protocol_version  
		 * 		Protocol version.
		 * 
		 * @return  
		 * 		True if succeeds.
		 */
		bool set_protocol_version(const std::string protocol_version);
		
		/**
		 * Set response body.
		 * 
		 * @param[in]  body  
		 * 		Response body string.
		 * 
		 * @return  
		 * 		True if succeeds.
		 */
		bool set_body(const std::string& body);
		
		/**
		 * Set response body length.
		 * 	
		 * @param body_length  
		 * 		Length of response body.
		 * 
		 * @return 
		 * 		True if succeeds.
		 */
		bool set_body_length(const std::streamoff body_length);
		
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
		 * 		True if succeeds.
		 */
		bool set_content_length(const size_t& new_content_length);
		
		/**
		 * Set content type.
		 * 
		 * @param[in] content_type  
		 * 		Content type.
		 */
		void set_content_type(const std::string& content_type);

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
		std::string generate_response();

		/**
		 * Clear up the header fields.
		 */
		void clear_up_header_fields();

	private:
		std::shared_ptr< Uri > m_uri;
		
		// Status code of response
		int m_status_code;

		// Default protocol version
		std::string m_protocol_version;
		
		// Reason phrase for specific status code.
		std::string m_reason_phrase;
		
		// Header fields
		std::map < std::string, std::string > m_headers;
		
		// Body of response message
		std::string m_body;
		
		// Length of body of response message
		size_t m_body_length;

		// Content type
		std::string m_content_type;
	};
}
