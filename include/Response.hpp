#pragma once
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <direct.h>	// directory manipulate
#endif

#ifdef __linux__
#include <unistd.h>
#endif

namespace Message
{
	class Response
	{
	public:
		Response(); 
		~Response() noexcept;
		Response(const Response&) noexcept;
		Response& operator=(const Response&) noexcept;
		Response(Response&&) noexcept;
		Response& operator=(Response&&) noexcept;

	public:
		bool hasHeader(const std::string& name);
		int getStatusCode();
		std::string getProtocolVersion();
		std::string getReasonPhrase();
		std::string get_body();
		std::string get_header(const std::string& header_name);
		std::string getStatusCodeReasonString(const int statusCode);
		std::string getBodyLength();
		int getBodyLengthInteger();
		std::string get_response_message();
		size_t get_response_length();

		/**
		 * @brief
		 * 		Set status code and corresponding reason phrase.
		 * @param[in]
		 * 		status code
		 * @return 
		 * 		bool
		 */
		bool setStatus(const int statusCodeInput);
		bool setProtocolVersion(const std::string versionProtocolInput);
		bool setBody(const std::string& bodyInput);
		bool setBodyLength(const std::streamoff bodyLengthInput);
		bool addHeader(const std::string& name, const std::string& value);

		/**
		 * set content-length to headers if length >= 0.
		 *
		 * @param[in] 
		 *		contentLengthInput is the size of given file in bytes;
		 *
		 * @return 
		 *		true, OK; false, Error.
		 */
		bool setContentLength(const std::streamoff& contentLengthInput);
		/**
		 * Maps the given Uri path onto the specified content type.
		 * according to file extention.
		 * https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 *
		 * @return 
		 *		true, if find the correspoinding content type.
		 *		false, if without type or not supported types.
		 */
		bool setContentType(const std::string& path);

		/** 
		 * Set the HTTP body and the corresponding headers.
		 *
		 * @return
		 *		true/false
		 */
		bool setContent(const std::string& path);

		/**
		 * Set the generated complete response message.
		 *
		 * @param[in] 
		 *		response is the generated complete response message.
		 *
		 * @return 
		 *		true/false.
		 */
		bool setResponseMessage(const std::string& response);
		
		/**
		 * Generate complete response message.
		 *
		 * @return 
		 *		true/false.
		 */
		bool generate_response();

		/**
		 * map the input path string to local file system
		 * as the payloal of response. It will set the body 
		 * and bodyLength private member variables.
		 *
		 * @param[in] 
		 *		path is the given path string extracted/parsed 
		 *		from request.
		 *
		 * @return 
		 *		ture/false to indicate success/error.
		 */
		bool readFile(const std::string& path);

		/**
		 * Convert given relative path to absolute path.
		 *
		 * @param[in] 
		 *		path is the given relative path.
		 *
		 * @param[out] 
		 *		absolutePath is the absolute path.
		 *
		 * @return
		 *		true/false
		 */
		bool convertPathToAbsolutePath(const std::string& path, std::string& absolutePath);


	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}


