#pragma once

#include <map>
#include <string>
#include <memory>
#include <sstream>

#include "CharacterSet.hpp"

class PercentEncoding
{
	// Life-cycle management
public:
	~PercentEncoding();
	PercentEncoding();

	PercentEncoding(const PercentEncoding&) = delete;
	PercentEncoding& operator=(const PercentEncoding&) = delete;

	PercentEncoding(PercentEncoding&&) = delete;
	PercentEncoding& operator=(PercentEncoding&&) = delete;

	// Member methods
public:

	/**
	 * Encode raw unencoded string.
	 * 
	 * @param[in]  unencoded_string  
	 * 		Raw unencoded string.
	 * 
	 * @return  
	 * 		Encoded string.
	 */
	std::string encode(const std::string& unencoded_string);

	/**
	 * Decode the given encoded_string.
	 * 
	 * @param[in]  encoded_string  
	 * 		String that has been encoded.
	 * 
	 * @return  
	 * 		Decoded string.
	 */
	std::string decode(const std::string& encoded_string);

private:
	/**
	 * Convert decimal integer to corresponding hexadecimal character.
	 * 
	 * @param[in] n 
	 * 		Given decimal integer.
	 * @return 
	 * 		Hexadecimal character [1-F]
	 */
	char convert_decimal_to_hexo_character(int n);

private:
	int decoded_character;
	int remaining_characters;
};
