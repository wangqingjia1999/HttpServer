#ifndef BASE64_HPP
#define BASE64_HPP
#include <memory>
#include <vector>
#include <string>

namespace base64
{
    /**
     * Encode the given string into Base64 string.
     * 
     * @param[in] unencoded_string
     *      Raw ASCII string.
     * @param[in] has_padding
     *      Whether has padding. Default is true.
     * @return std::string
     *      The encoded Base64 string.
     */
    std::string encode(const std::string& unencoded_string, bool has_padding = true);

    /**
     * Encode the hexadecimal string into Base64 string.
     * 
     * @param[in] hex_string
     *      Hexadecimal string.
     * @param[in] has_padding
     *      Whether has padding. Default is true.
     * @return std::string
     *      The encoded Base64 string.
     */
    std::string encode_hex_string(const std::string& hex_string, bool has_padding = true);

    /**
     * Decode the given Base64 string.
     * 
     * @param[in] encoded_string
     *      Base64 string.
     * @param[in] has_padding
     *      Whether has padding. Default is true.
     * @return std::string
     *      The unencoded/decoded string.
     */
    std::string decode(const std::string& encoded_string, bool has_padding = true); 
    
    /**
     * Encode the given URL string into Base64 URL string.
     * 
     * @param[in] unencoded_uri_string
     *      The unencoded URL string.
     * @param[in] has_padding
     *      Whether has padding. Default is true.
     * @return std::string
     *      The encoded URL string.
     */
    std::string encode_url(const std::string& unencoded_uri_string, bool has_padding = true);
    
    /**
     * Decode the given Base64 encoded string.
     * 
     * @param[in] encoded_uri_string
     *      The encoded URL string.
     * @param[in] has_padding
     *      Whether has padding. Default is true.
     * @return std::string
     *      The decoded URL string.
     */
    std::string decode_url(const std::string& encoded_uri_string, bool has_padding = true);
}
    
#endif