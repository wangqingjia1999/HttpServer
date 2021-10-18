#pragma once

#include <memory>
#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace Utf8
{

	/**
	 * This represents a single character in Unicode.
	 */
	typedef uint32_t UnicodeCodePoint;

	/**
	 * converting ASCII string to its equivalent sequence of Unicode code
	 * points.
	 *
	 * @param[in] ascii
	 *      ASCII string.
	 *
	 * @return
	 *      The Unicode code points sequence.
	 */
	std::vector<UnicodeCodePoint> AsciiToUnicode(const std::string& ascii);

	/**
	 * This class is used to encode or decode Unicode "code points",
	 * or characters from many different international character sets,
	 * in order to store or transmit them across any interface
	 * that accepts a sequence of bytes.
	 */
	class Utf8
	{
		// Lifecycle management
	public:
		~Utf8() noexcept;
		Utf8();

		Utf8(const Utf8&) = delete;
		Utf8& operator=(const Utf8&) = delete;

		Utf8(Utf8&&) noexcept = delete;
		Utf8& operator=(Utf8&&) noexcept = delete;

	public:
		/**
		 * Encode Unicode code points to UTF-8 sequence.
		 *
		 * @param[in] code_points
		 *      Unicode code points.
		 *
		 * @return
		 *      The UTF-8 encoding sequence.
		 */
		static std::vector<uint8_t>
		Encode(const std::vector<UnicodeCodePoint>& code_points);

		/**
		 * Decode UTF-8 sequence to Unicode sequence.
		 *
		 * @param[in] encoding
		 *      UTF-8 encoded bytes sequence.
		 *
		 * @return  Unicode code points sequence.
		 *
		 * @note
		 *      This function accepts the given sequence of UTF-8 encoded bytes,
		 *      and returns any Unicode code points formed from them.
		 *
		 *      Any partial code sequence at the end is held onto and used first
		 *      when this method is called again later.
		 */
		std::vector<UnicodeCodePoint>
		Decode(const std::vector<uint8_t>& encoding);

		/**
		 * Decode UTF-8 sequence to Unicode sequnce.
		 *
		 * @param[in] encoding
		 *      UTF-8 encoded sequence.
		 *
		 * @return
		 *      Unicode code points.
		 */
		std::vector<UnicodeCodePoint> Decode(const std::string& encoding);

		/**
		 * Whether it is UTF-8 encoding?
		 *
		 * @param[in] encoding
		 *      UTF-8 encoded bytes sequence.
		 *
		 * @param[in] final
		 *      A flag indicates whether or not this is the end of the encoding.
		 *
		 * @return
		 *      True if it is UTF-8 encoding.
		 *
		 * @note
		 *      If the final is false, any partial code sequence at the end
		 *      is held onto and used first when this method is called again
		 * later.
		 */
		bool IsValidEncoding(const std::string& encoding, bool final = true);

	private:
		/**
		 * This is the type of structure that contains the private
		 * properties of the instance.  It is defined in the implementation
		 * and declared here to ensure that it is scoped inside the class.
		 */
		struct Impl;

		/**
		 * This contains the private properties of the instance.
		 */
		std::unique_ptr<Impl> impl_;
	};

} // namespace Utf8
