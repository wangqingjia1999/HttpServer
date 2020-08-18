#pragma once
#include <string>
#include <memory>

namespace Uri
{
	class PercentEncoding
	{
		// Lifecycle management
	public:
		~PercentEncoding();
		PercentEncoding();

		PercentEncoding(const PercentEncoding&);
		PercentEncoding& operator=(const PercentEncoding&) ;

		PercentEncoding(PercentEncoding&&) noexcept;
		PercentEncoding& operator=(PercentEncoding&&) noexcept;

		// Member methods
	public:

		/**
		 * @brief  Encode raw unencoded string.
		 * @param  unencoded_string  Raw unencoded string.
		 * @return  Encoded string.
		 */
		std::string encode(const std::string& unencoded_string);

		/**
		 * @brief  Decode the given encoded_string.
		 * @param  encoded_string  String has been encoded.
		 * @return  Decoded string.
		 */
		std::string decode(const std::string& encoded_string);

		/**
		 * @brief  Is finished processing?
		 * @return  True if it is finished.
		 */
		bool is_finished();

		/**
		 * @brief  Get decoded character.
		 * @return  Decoded character.
		 */
		char get_decoded_character();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}