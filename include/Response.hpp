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
		 * 		Set status code and corresponding reason phrase.
		 * @param[in]
		 * 		status code
		 * @return
		 * 		true if succeeds;
		 * 		false if fails.
		 */
		bool set_status(const int status_codeInput);
		bool set_protocol_version(const std::string versionProtocolInput);
		bool set_body(const std::string& bodyInput);
		bool set_body_length(const std::streamoff body_lengthInput);
		bool add_header(const std::string& name, const std::string& value);

		/**
	     * Set content-length to headers if length >= 0.
		 * 
		 * @param[in] 
		 *		contentLengthInput is the size of given file in bytes;
		 * @return 
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool set_content_length(const std::streamoff& contentLengthInput);
		
		/**
	     * Maps the given Uri path onto the specified content type according to file extention.
		 * https://www.iana.org/assignments/media-types/media-types.xhtml#application
		 * 
		 * @return 
		 *		true if find the correspoinding content type.
		 *		false if doesn't have type or currently not supported.
		 */
		bool set_content_type(const std::string& path);

		/** 
		 * Set the Response's entity body and the corresponding entity header fields.
		 *
		 * @return
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool set_content(const std::string& path);

		/**
		 * Set the generated complete response message.
		 *
		 * @param[in] 
		 *		response is the generated complete response message.
		 * @return 
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool set_response_message(const std::string& response);
		
		/**
		 * 		Set reason phrase.
		 * @param[in]
		 * 		Status code integer.
		 * @return 
		 * 		true if succeeds;
		 * 		false if fails.
		 */
		bool set_reason_phrase(const int stauts_code);

		/**
		 * Generate/Assemble response string and store the string into member variable.
		 *
		 * @return 
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool generate_response();

		/**
		 * Map the input path string to local file system as the payloal of response. 
		 * It will set the body and body length as well.
		 * 
		 * @param[in] path
		 *		Path string parsed from request string.
		 * @return
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool read_file(const std::string& path);

		/**
		 * 	Convert given relative path to absolute path.
		 * 
		 * @param[in] path
		 *		Relative path.
		 * @param[out] absolute_path
		 *		Absolute path.
		 * @return
		 *		true if succeeds;
		 *		false if fails.
		 */
		bool convert_path_to_absolute(const std::string& path, std::string& absolute_path);

		/**
		 * Clear up the header fields.
		 */
		void clear_up_header_fields();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}

#endif

