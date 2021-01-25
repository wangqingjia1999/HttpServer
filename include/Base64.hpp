#pragma once

#include <memory>
#include <vector>
#include <string>

namespace Base64
{
    /**
     * @brief  Encode the given string into Base64 string.
     * @param  unencoded_string  Raw ASCII string.
     * @param  has_padding  Whether has padding. Default is true.
     * @return  The encoded Base64 string.
     */
    std::string encode(const std::string& unencoded_string, bool has_padding = true);

    /**
     * @brief  Encode the hexadecimal string into Base64 string.
     * @param  hex_string  Hexadecimal string.
     * @param  has_padding  Whether has padding. Default is true.
     * @return  The encoded Base64 string.
     */
    std::string encode_hex_string(const std::string& hex_string, bool has_padding = true);

    /**
     * @brief  Decode the given Base64 string.
     * @param  encoded_string  Base64 string.
     * @param  has_padding  Whether has padding. Default is true.
     * @return  The unencoded/decoded string.
     */
    std::string decode(const std::string& encoded_string, bool has_padding = true); 
    
    /**
     * @brief  Encode the given URL string into Base64 URL string.
     * @param  unencoded_uri_string  The unencoded URL string.
     * @param  has_padding  Whether has padding. Default is true.
     * @return  The encoded URL string.
     */
    std::string encode_url(const std::string& unencoded_uri_string, bool has_padding = true);
    
    /**
     * @brief  Decode the given Base64 encoded string.
     * @param  encoded_uri_string  The encoded URL string.
     * @param  has_padding  Whether has padding. Default is true.
     * @return  The decoded URL string.
     */
    std::string decode_url(const std::string& encoded_uri_string, bool has_padding = true);
}