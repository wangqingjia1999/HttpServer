#pragma once
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "Request.hpp"

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
		 * @brief
		 * 		Set status code and corresponding reason phrase.
		 * @param[in]
		 * 		status code
		 * @return 
		 * 		bool
		 */
		bool set_status(const int status_codeInput);
		bool set_protocol_version(const std::string versionProtocolInput);
		bool set_body(const std::string& bodyInput);
		bool set_body_length(const std::streamoff body_lengthInput);
		bool add_header(const std::string& name, const std::string& value);

		/**
		 * @brief
		 * 		Set content-length to headers if length >= 0.
		 * @param[in] 
		 *		contentLengthInput is the size of given file in bytes;
		 * @return 
		 *		true, OK; false, Error.
		 */
		bool set_content_length(const std::streamoff& contentLengthInput);
		
		/**
		 * @brief
	     *		Maps the given Uri path onto the specified content type according to file extention.
		 *		https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 * @return 
		 *		true, if find the correspoinding content type.
		 *		false, if without type or not supported types.
		 */
		bool set_content_type(const std::string& path);

		/** 
		 * Set the HTTP body and the corresponding headers.
		 *
		 * @return
		 *		true/false
		 */
		bool set_content(const std::string& path);

		/**
		 * Set the generated complete response message.
		 *
		 * @param[in] 
		 *		response is the generated complete response message.
		 *
		 * @return 
		 *		true/false.
		 */
		bool set_response_message(const std::string& response);
		
		/**
		 * @brief
		 * 		Set reason phrase.
		 * @param[in]
		 * 		Status code integer.
		 * @return 
		 * 		bool.
		 */
		bool set_reason_phrase(const int stauts_code);

		/**
		 * @brief
		 * 		Generate complete response message.
		 * @return 
		 *		true/false.
		 */
		bool generate_response();

		/**
		 * @brief
		 * 		map the input path string to local file system
		 * 		as the payloal of response. It will set the body 
		 * 		and body length as well.
		 * @param[in] 
		 *		path is the given path string extracted/parsed 
		 *		from request.
		 * @return 
		 *		bool.
		 */
		bool read_file(const std::string& path);

		/**
		 * @brief
		 * 		Convert given relative path to absolute path.
		 * @param[in] 
		 *		path is the given relative path.
		 * @param[out] 
		 *		absolute_path is the absolute path.
		 * @return
		 *		bool.
		 */
		bool convert_path_to_absolute(const std::string& path, std::string& absolute_path);

		/**
		 * @brief
		 * 		Status Code Handler.
		 * @param[in]
		 * 		status code integer.
		 */
		void handle_status_code(const int status_code);

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}


