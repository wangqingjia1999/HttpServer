#include "base64.hpp"
#include "logger.hpp"

#include <vector>
#include <sstream>
#include <map>
#include <bitset>
#include <iostream>

namespace
{
    /**
     * For general Base64 encoding.
     * Convert one 6-bit input group(in the form of decimal number) 
     * to its corresponding Base64 character.
     */
    const char encoding_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    /**
     * For general Base64 decoding.
     * Map a encoded Base64 character to its corresponding 6-bit group(in the form of decimal number)
     */
    const std::map< char, uint8_t > decoding_map{
        {'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},  {'E', 4},  {'F', 5},  {'G', 6},  {'H', 7},
        {'I', 8},  {'J', 9},  {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
        {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
        {'Y', 24}, {'Z', 25}, {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31},
        {'g', 32}, {'h', 33}, {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39},
        {'o', 40}, {'p', 41}, {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47},
        {'w', 48}, {'x', 49}, {'y', 50}, {'z', 51}, {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55},
        {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59}, {'8', 60}, {'9', 61}, {'+', 62}, {'/', 63},
    };

    /**
     * Only for URL Base64 encoding.
     * Convert one 6-bit input group(in the form of decimal number) 
     * to its corresponding Base64 character.
     */
    const char url_encoding_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    /**
     * Only for URL Base64 decoding.
     * Map a encoded Base64 character to its corresponding 6-bit group(in the form of decimal number)
     */
    const std::map< char, uint8_t > url_decoding_map{
        {'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},  {'E', 4},  {'F', 5},  {'G', 6},  {'H', 7},
        {'I', 8},  {'J', 9},  {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
        {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
        {'Y', 24}, {'Z', 25}, {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31},
        {'g', 32}, {'h', 33}, {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39},
        {'o', 40}, {'p', 41}, {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47},
        {'w', 48}, {'x', 49}, {'y', 50}, {'z', 51}, {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55},
        {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59}, {'8', 60}, {'9', 61}, {'-', 62}, {'_', 63},
    };

    /**
     * Convert Hexadecimal string to byte sequence.
     * Based on https://stackoverflow.com/a/30606613/13018017
     * 
     * @param[in] hex_string
     *      Hexadecimal string.
     * @return std::vector< uint8_t >
     *      Byte sequence in the form of std::vector< uint8_t >.
     */
    std::vector< uint8_t > hex_string_to_byte_array(const std::string& hex_string)
    {
        std::vector< uint8_t > byte_sequence;

        // If length of string is odd, add one 0 to the end.
        std::string new_hex_string;
        if(byte_sequence.size() % 2 != 0)
        {
            new_hex_string = hex_string;
            char last_character = hex_string[hex_string.size() - 1];
            new_hex_string.pop_back();
            new_hex_string += '0';
            new_hex_string += last_character;
        }
        else // The length of string is even, we do not need to modify it.
        {
            new_hex_string = hex_string;    
        }
    
        for(int i = 0; i < new_hex_string.size(); i += 2)
        {
            std::string buffer = new_hex_string.substr(i, 2);
            uint8_t byte = (uint8_t)std::stol(buffer.c_str(), nullptr, 16);
            byte_sequence.push_back(byte);
        }
        return byte_sequence;
    }

    /**
     * Encode the given unencoded ASCII string.
     * 
     * @param[in] unencoded_string
     *      The unencoded ASCII string.
     * @param[in] is_url_encoding
     *      Whether is url encoding. Default is false.
     * @param[in] is_hex_string
     *      Whether is hexadecimal string. Default is false. 
     * @param[in] has_padding
     *      Whether has padding. Default is ture.
     */
    std::string encode_implementation(
        const std::string& unencoded_string, 
        bool is_url_encoding = false,
        bool is_hex_string = false,
        bool has_padding = true
    )
    {
        size_t number_of_bytes = unencoded_string.size();
        std::vector< uint8_t > bytes_sequence;
        if(!is_hex_string)
        {
            for(auto character : unencoded_string)
            {
                bytes_sequence.push_back(character);
            }
        }
        else // is hexadecimal string
        {
            /**
             * If is hexadecimal string, each char is 4-bit,
             * So the string's length should be string.size() / 2.
             */
            number_of_bytes = unencoded_string.size() / 2;
            bytes_sequence = hex_string_to_byte_array(unencoded_string);
        }   
           
        size_t bits = 0;
        uint16_t buffer = 0;
        std::stringstream output;
        // main loop
        for(auto byte : bytes_sequence)
        {
            buffer <<= 8;
            buffer += (uint16_t)byte;
            bits += 8;
            while ( bits >= 6)
            {
                if(is_url_encoding)
                {
                    /**
                     * 0x3f is 0011 1111
                     * 1101 0101 & 0x3f => 0001 0101 (reserve the most significant 6 bits of 1101 0101)
                     * 1101 0101 >> 2 => 0011 0101 (drop the least significant 2 bits)
                     */
                    output << url_encoding_table[(buffer >> (bits-6)) & 0x3f];
                }
                else
                {
                    output << encoding_table[(buffer >> (bits-6)) & 0x3f];
                }
                
                /**
                 * clear most significant 6 bits of buffer.
                 */
                buffer &= ~(0x3f << (bits-6));
                bits -= 6;
            }
        }

        /**
         * If after main loop, there are still (1 byte - 6 bits = 2 bit)
         * Shift left 4 bits to form a new 6-bit character
         * and add padding(=) if needed.
         */
        if(number_of_bytes % 3 == 1)
        {
            buffer <<= 4;
            if(is_url_encoding)
            {
                output << url_encoding_table[buffer & 0x3f];
                if(has_padding)
                {
                    output << "==";
                }
            }
            else // is not URL encoding
            {
                output << encoding_table[buffer & 0x3f];
                if(has_padding)
                {
                    output << "==";
                }
            }
        }
        else if (number_of_bytes % 3 == 2)
        {
             /**
             * If after main loop, there are still (2 bytes - 12 bits = 4 bit)
             * Shift left 2 bits to form a new 6-bit character.
             * and add padding if needed.
             */
            buffer <<= 2;
            if(is_url_encoding)
            {
                output << url_encoding_table[buffer & 0x3f];
                if(has_padding)
                {
                    output << '=';
                }
            }
            else // is not URL encoding
            {
                output << encoding_table[buffer & 0x3f];  
                if(has_padding)
                {
                    output << '=';
                }  
            }
        }

        return output.str();
    }

    /**
     * Decode the given encoded Base64 string.
     * 
     * @param[in] encoded_string
     *      The encoded Base64 string.
     * @param[in] is_url_encoding
     *      Default is false.
     * @param[in] has_padding
     *      Default is ture.
     */
    std::string decode_implementation(
        const std::string& encoded_string, 
        bool is_url_decoding = false,
        bool has_padding = true
    )
    {
        std::vector< uint8_t > bytes_sequence;
        for(auto character : encoded_string)
        {
            bytes_sequence.push_back(character);
        }

        std::stringstream output;
        size_t bits = 0;
        uint16_t buffer = 0;
        for(auto character : bytes_sequence)
        {
            
            // group: 6 bits group
            uint8_t group = 0;
            if(is_url_decoding)
            {
                if(url_decoding_map.find(character) != url_decoding_map.end())
                {
                    group = url_decoding_map.find(character)->second;
                }
            }
            else
            {
                if(decoding_map.find(character) != decoding_map.end())
                {
                    group = decoding_map.find(character)->second;
                }
            }
            
            buffer <<= 6;
            bits += 6;
            buffer += group;
            if(bits >= 8)
            {
                if(character != '=')
                {
                    output << (char)(buffer >> (bits-8));
                }
                buffer &= ~(0xff << (bits-8));
                bits -= 8;
            }
        }
        return output.str();
    }
}

namespace base64
{
    std::string encode(const std::string& unencoded_string, bool has_padding)
    {
        return encode_implementation(unencoded_string, false, false, has_padding);
    }

    std::string encode_hex_string(const std::string& hex_string, bool has_padding)
    {
        return encode_implementation(hex_string, false, true, has_padding);
    }

    std::string decode(const std::string& encoded_string, bool has_padding)
    {
        return decode_implementation(encoded_string, false, has_padding);
    }

    std::string encode_url(const std::string& unencoded_string, bool has_padding)
    {
        return encode_implementation(unencoded_string, true, false, has_padding);
    }

    std::string decode_url(const std::string& encoded_string, bool has_padding)
    {
        return decode_implementation(encoded_string, true, has_padding);
    }
}

