#include "Percent_Encoding.hpp"

Percent_Encoding::~Percent_Encoding()
{

}

Percent_Encoding::Percent_Encoding() 
	: decoded_character(0), 
	  remaining_characters(0)
{
}

std::string Percent_Encoding::encode(const std::string& unencoded_string)
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
			encoded_string.push_back(convert_decimal_to_hexo_character(first));
			encoded_string.push_back(convert_decimal_to_hexo_character(second));
		}
		else
		{
			encoded_string.push_back(unencoded_string[i]);
		}
	}
	return  encoded_string;
}


std::string Percent_Encoding::decode(const std::string& encoded_string)
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

bool Percent_Encoding::is_finished()
{
	return remaining_characters == 0;
}

char Percent_Encoding::get_decoded_character()
{
	return char(decoded_character);
}

char Percent_Encoding::convert_decimal_to_hexo_character(int n)
{
	return hexMap.find(n)->second;
}
