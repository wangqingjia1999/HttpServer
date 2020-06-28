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
		bool generateRequest();

		/**
		 * @brief
		 * 		Parse given request line(first line of request message).
		 *
		 * @param[in] 
		 *		requestLine is the given request line.
		 *
		 * @return
		 *		true/false.
		 */
		bool parserequestLine(const std::string& requestLine);

		/** 
		 * @brief
		 * 		Parse given headers.
		 * @param[in] 
		 *		headers is the map that stores name-value
		 *		header pairs.
		 * @return 
		 *		true/false.
		 */
		bool parseHeaders(const std::string& headers);

		/**
		 * @brief
		 * 		Parse raw request that has already set by setRawRequest()
		 * @return 
		 *		true/false.
		 */
		bool parseRawRequest();

		/**
		 * @brief
		 * 		Set received raw request message to request object's member variable.
		 * @param[in]
		 * 		Received raw request message string.
		 * @param[out] 
		 * 		Object request's member rawRequest is set.
		 * @return 
		 * 		true/false
		 */
		bool setRawRequest(std::string rawRequestString);

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


