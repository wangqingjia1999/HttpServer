#ifndef PERCENT_ENCODING_HPP
#define PERCENT_ENCODING_HPP

#include <map>
#include <string>
#include <memory>
#include <sstream>

#include "Character_Set.hpp"

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
	char convert_decimal_to_hexo_character(int n);

private:
	int decoded_character;

	int remaining_characters;

	// gen-delims characters
	const Character_Set GEN_DELIMS
	{
		':', '/', '?' , '#' , '[' , ']' , '@',
	};

	// sub-delims characters
	const Character_Set SUB_DELIMS
	{
		'!', '$', '&', '\'', '(', ')',
		'*', '+', ',', ';', '=',
	};

	const Character_Set RESERVED
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
};

#endif