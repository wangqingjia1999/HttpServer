#pragma once

#include "Uri.hpp"
#include "Logger.hpp"

#include <stdio.h>

#include <map>
#include <memory>
#include <sstream>
#include <algorithm>

namespace Message
{
	class Request
	{
	public:
		~Request() noexcept;
		Request();

		Request& operator=(const Request&);
		Request(const Request&);
		
		Request(Request&&) noexcept;
		Request& operator=(Request&&) noexcept;

	public:
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
		bool has_http_version() const;
		bool has_method(const std::string& method) const;	
		bool has_query() const;
				
		void set_raw_request(std::string raw_request_string);
		void set_method(const std::string new_method = "GET");
		void set_http_version(const std::string new_http_version = "HTTP/1.1");
		void set_user_agent(std::string new_user_agent = "Bitate");

		std::string get_raw_request();
		std::string get_request_method();
		std::shared_ptr<Uri> get_request_uri();
		std::string get_http_version();
		std::string get_header(const std::string& header_name);
		std::string get_body();
		std::string get_generated_request();
		std::string get_host();
		std::string get_port();

	private:
		/**
         * For search bar: /?Search=This+is+just+a+demo
         * For normal get: /index.html
         * For more info, see: https://tools.ietf.org/html/rfc2616#section-5.1.2
         */
		std::shared_ptr< Uri > m_uri;
        
        // Generated/Received raw request string
        std::string m_raw_request;

        // Request method. e.g. GET, POST
        std::string m_method;
        
        // default http_version
        std::string m_http_version;

        // Store the generated/received raw headers
        std::string m_headers;

        // Contain the header {key: value} pairs
        std::map< std::string, std::string> m_headers_map;

        // request body string
        std::string m_body;
	};
}
