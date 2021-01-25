#pragma once

#include "Uri.hpp"
#include "Logger.hpp"

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
		 * Parse Uri.
		 * 	
		 * @param[in] Uri 
		 * 		Uri string.
		 * 	
		 * @return  
		 * 		True if successfully parse the given Uri string.
		 */
		bool parse_uri(const std::string& Uri);
		
		void set_raw_request(std::string raw_request_string);
		void set_method(const std::string new_method = "GET");
		void set_http_version(const std::string new_http_version = "HTTP/1.1");
		void set_user_agent(std::string new_user_agent = "Bitate");

		/**
		 * Get request method string.
		 * 
		 * @return 
		 * 		Method string of request message.
		 */
		std::string get_request_method();

		/**
		 * Get request Uri string.
		 * 
		 * @return  
		 * 		Request Uri string.
		 */
		std::string get_request_uri();

		/**
		 * Get request http protocol version.
		 * 
		 * @return  
		 * 		Http protocol version string.
		 */
		std::string get_http_version();

		/**
		 * Get given request header.
		 * 
		 * @return  
		 * 		Header name's corresponding header value.
		 */
		std::string get_header(const std::string& header_name);

		/**
		 * Get request body string.
		 * 	
		 * @return 
		 * 		Request body string.
		 */
		std::string get_body();

		/**
		 * Get generated request string.
		 * 
		 * @return  
		 * 		Request string.
		 */
		std::string get_generated_request();

		/**
		 * Has given header?
		 * 	
		 * @return  
		 * 		True if it has.
		 */
		bool has_header(const std::string& header_name) const;

		/**
		 * Has http version field set?
		 * 
		 * @return  
		 * 		True if it sets.
		 */
		bool has_http_version() const;

		/**
		 * Has request method set? 
		 * 
		 * @return 
		 * 		True if it sets.
		 */
		bool has_method(const std::string& method) const;
		
		/**
		 * Get raw request.
		 * 
		 * @return  
		 * 		Raw request string.
		 */
		std::string get_raw_request();

		/**
		 * Get host name.
		 * 
		 * @return  
		 * 		Host string.
		 */
		std::string get_host();

		/**
		 * Get port string.
		 * 
		 * @return  
		 * 		Port string.
		 */
		std::string get_port();

	private:
		Logger logger;

		std::shared_ptr< Uri > uri = std::make_shared< Uri > ();
        
        // Generated/Received raw request string
        std::string raw_request;

        // Request method. e.g. GET, POST
        std::string method;
        
        // default http_version
        std::string http_version = "HTTP/1.1";

        // Store the generated/received raw headers
        std::string headers;

        /**
         * For search bar: /?Search=This+is+just+a+demo
         * For normal get: /index.html
         * For more info, see: https://tools.ietf.org/html/rfc2616#section-5.1.2
         */
        std::string request_uri;

        // Contain the header {key: value} pairs
        std::map< std::string, std::string> headers_map;

        // request body string
        std::string body;
	};
}
