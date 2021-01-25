#ifndef PERCENT_ENCODING_HPP
#define PERCENT_ENCODING_HPP

#include <map>
#include <string>
#include <memory>
#include <sstream>

#include "CharacterSet.hpp"

class Percent_Encoding
{
	// Life-cycle management
public:
	~Percent_Encoding();
	Percent_Encoding();

	Percent_Encoding(const Percent_Encoding&) = delete;
	Percent_Encoding& operator=(const Percent_Encoding&) = delete;

	Percent_Encoding(Percent_Encoding&&) = delete;
	Percent_Encoding& operator=(Percent_Encoding&&) = delete;

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

#endif