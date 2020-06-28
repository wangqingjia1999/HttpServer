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
		// TODO: this is not a complete method.
		std::string generateRequest();

		/**
		 * Parse given request line(first line of request message).
		 *
		 * @param[in] 
		 *		requestLine is the given request line.
		 *
		 * @return
		 *		true/false.
		 */
		bool parserequestLine(const std::string& requestLine);

		/** 
		 * Parse given headers.
		 *
		 * @param[in] 
		 *		headers is the map that stores name-value
		 *		header pairs.
		 *
		 * @return 
		 *		true/false.
		 */
		bool parseHeaders(const std::string& headers);

		/**
		 * Parse raw request received from client.
		 * 
		 * @param[in] 
		 *		rawRequest is the raw request.
		 *
		 * @return 
		 *		true/false.
		 */
		bool parseRawRequest(const std::string& rawRequest);


		std::string getMethod();
		std::string getRequestUri();
		std::string getHttpVersion();
		std::string getHeader(const std::string& headerName);
		std::string getBody();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}


