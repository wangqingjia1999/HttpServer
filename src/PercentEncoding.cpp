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

	// Temporary solution
	// URI producers and normalizers should use uppercase hexadecimal digits
	// for all percent-encodings.
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

	Uri::PercentEncoding::PercentEncoding(const PercentEncoding& other) noexcept
	{
		*this = other;
	}

	PercentEncoding& Uri::PercentEncoding::operator=(const PercentEncoding& other) noexcept
	{
		if(this != &other)
		{
			*impl_ = *other.impl_;
		}
		return *this;
	}

	Uri::PercentEncoding::PercentEncoding(PercentEncoding&&) noexcept = default;
	PercentEncoding& Uri::PercentEncoding::operator=(PercentEncoding&&) noexcept = default;

	std::string Uri::PercentEncoding::Encode(const std::string& s)
	{
		// Under normal circumstances, the only time to encode 

		// is during the process of producing the URI from components.

		// "%" is the indicater of percent-encoded octets.

		// Implementations must not encode/decode the same string more than once

		// Encode two steps:
		// 1. Convert the character string into a sequence of bytes using the UTF-8 encoding
		// 2. Convert each byte that is not an ASCII letter or digit to % HH, where HH is the 
		//	  hexadecimal value of the byte

		std::string encodedString;

		for (size_t i = 0; i < s.size(); ++i)
		{
			// only encode reserved characters
			if (RESERVED.isContains(s[i]))
			{
				int second = s[i] % 16;
				int first = (s[i] - second) / 16;

				encodedString.push_back('%');
				encodedString.push_back(convertDecimalToHexoCharacter(first));
				encodedString.push_back(convertDecimalToHexoCharacter(second));
			}
			else
			{
				encodedString.push_back(s[i]);
			}
		}
		return  encodedString;
	}

	/**
	 * Decode the given encodedString.
	 *
	 * @note
	 *		Only support ASCII characters.
	*
	* @param[in] s
	*		is the given encodedString.
	*
	* @return
	*		DecodedString.
	*/
	std::string Uri::PercentEncoding::Decode(const std::string& s)
	{
		// When a URI is dereferenced/Decode, the components and subcomponents
		// must be parsed and separated before the decoding.
		// Otherwise, the data may be mistaken from component delimiters.

		// The uppercase hexadecimal digits 'A' through 'F' are equivalent to
		// the lowercase digits 'a' through 'f', respectively.
		if (s.find('%') == std::string::npos)
		{
			return s;
		}

		std::string decodedString;

		for (size_t i = 0; i < s.size(); ++i)
		{
			// %[1][2] 
			// [1]*16 + [2] 
			// This way, we can get its corespoding decimal number,
			// Thus, directly turn it into char.

			// TODO: Add toUper string extension
			if (s[i] == '%')
			{
				char first = s[i + 1];
				char second = s[i + 2];
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
				decodedString.push_back(decodedChar);
				decodedChar = '\0';
			}
			else
			{
				decodedString.push_back(s[i]);
			}
		}

		return decodedString;
	}

	bool Uri::PercentEncoding::isFinished()
	{
		if (impl_->remainingCharacters == 0)
		{
			return true;
		}
		return false;
	}

	char Uri::PercentEncoding::getDecodedeCharacter()
	{
		return char(impl_->decodedCharacter);
	}

}