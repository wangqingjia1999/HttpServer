#include "PercentEncoding.hpp"
#include "CharacterSet.hpp"
#include <sstream>
#include <map>

namespace
{
	// gen-delims characters
	const Uri::CharacterSet GEN_DELIMS
	{
		':', '/', '?' , '#' , '[' , ']' , '@',
	};

	// sub-delims characters
	const Uri::CharacterSet SUB_DELIMS
	{
		'!', '$', '&', '\'', '(', ')',
		'*', '+', ',', ';', '=',
	};

	const Uri::CharacterSet RESERVED
	{
		GEN_DELIMS,
		SUB_DELIMS,
	};

	/**
	 * Experimental solution:
	 * URI producers and normalizers should use uppercase hexadecimal digits
	 * for all percent-encodings.
	 */
	const std::map<int, char> hexMap
	{
		{0,'0'},
		{1,'1'},
		{2,'2'},
		{3,'3'},
		{4,'4'},
		{5,'5'},
		{6,'6'},
		{7,'7'},
		{8,'8'},
		{9,'9'},
		{10,'A'},
		{11,'B'},
		{12,'C'},
		{13,'D'},
		{14,'E'},
		{15,'F'},
	};

	char convertDecimalToHexoCharacter(int n)
	{
		return hexMap.find(n)->second;
	}
}

namespace Uri
{
	struct Uri::PercentEncoding::Impl
	{
		int decodedCharacter = 0;

		int remainingCharacters = 0;
	};

	Uri::PercentEncoding::~PercentEncoding() = default;
	Uri::PercentEncoding::PercentEncoding() : impl_(new Impl)
	{
	}

	Uri::PercentEncoding::PercentEncoding(const PercentEncoding& other)
	{
		*this = other;
	}

	PercentEncoding& Uri::PercentEncoding::operator=(const PercentEncoding& other)
	{
		if(this != &other)
		{
			*impl_ = *other.impl_;
		}
		return *this;
	}

	Uri::PercentEncoding::PercentEncoding(PercentEncoding&&) noexcept = default;
	PercentEncoding& Uri::PercentEncoding::operator=(PercentEncoding&&) noexcept = default;

	std::string Uri::PercentEncoding::encode(const std::string& unencoded_string)
	{
		/**
		 * Under normal circumstances, the only time to encode is during the process
		 * of assembling the URI from its components.
		 * '%' is the indicator of percent-encoding.
		 * 
		 * Two steps to encode:
		 * 	1. Convert the character string into a sequence of bytes using the UTF-8 encoding;
		 * 	2. Convert each byte that is not an ASCII letter or digit to % HH, where HH is 
		 * 	   the hexadecimal value of the byte
		 */
		std::string encoded_string;
		for (size_t i = 0; i < unencoded_string.size(); ++i)
		{
			// only encode reserved characters
			if (RESERVED.is_contains(unencoded_string[i]))
			{
				int second = unencoded_string[i] % 16;
				int first = (unencoded_string[i] - second) / 16;

				encoded_string.push_back('%');
				encoded_string.push_back(convertDecimalToHexoCharacter(first));
				encoded_string.push_back(convertDecimalToHexoCharacter(second));
			}
			else
			{
				encoded_string.push_back(unencoded_string[i]);
			}
		}
		return  encoded_string;
	}

	
	std::string Uri::PercentEncoding::decode(const std::string& encoded_string)
	{
		// When a URI is dereferenced/decode, the components and subcomponents
		// must be parsed and separated before the decoding.
		// Otherwise, the data may be mistaken from component delimiters.

		// The uppercase hexadecimal digits 'A' through 'F' are equivalent to
		// the lowercase digits 'a' through 'f', respectively.
		if (encoded_string.find('%') == std::string::npos)
		{
			return encoded_string;
		}

		std::string decoded_uri_string;
		for (size_t i = 0; i < encoded_string.size(); ++i)
		{
			/**
			 * Each character is in the form of: %[1][2]
			 * We do this:  [1]*16 + [2]
			 * Use this method, we get the decimal number,
			 * then directly converting it to char.
			 * 
			 * TODO:  Add to_upper_case().
			 */
			if (encoded_string[i] == '%')
			{
				char first = encoded_string[i + 1];
				char second = encoded_string[i + 2];
				i = i + 2;
				if (first >= 'A')
				{
					first = first - 'A' + 10;
				}
				else if (first >= '0')
				{
					first = first - '0';
				}

				if (second >= 'A')
				{
					second = second - 'A' + 10;
				}
				else if (second >= '0')
				{
					second = second - '0';
				}

				// decimal number
				int decimal = first * 16 + second;
				char decodedChar = char(first * 16 + second);

				// add to decoded string
				decoded_uri_string.push_back(decodedChar);
				decodedChar = '\0';
			}
			else
			{
				decoded_uri_string.push_back(encoded_string[i]);
			}
		}

		return decoded_uri_string;
	}

	bool Uri::PercentEncoding::is_finished()
	{
		if (impl_->remainingCharacters == 0)
		{
			return true;
		}
		return false;
	}

	char Uri::PercentEncoding::get_decoded_character()
	{
		return char(impl_->decodedCharacter);
	}

}