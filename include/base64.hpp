#ifndef BASE64_HPP
#define BASE64_HPP
#include <memory>


namespace base64
{
/**
     * Encode the given string into Base64 string.
     * 
     * @param[in] unencoded_string
     *      Raw ASCII string.
     * @param[in] has_padding
     *      Default is false.
     * @return std::string
     *      The encoded Base64 string.
     */
    std::string encode(const std::string& unencoded_string, bool has_padding=false);

    /**
     * Decode the given Base64 string.
     * 
     * @param[in] encoded_string
     *      Base64 string.
     * @param[in] has_padding
     *      Default is false.
     * @return std::string
     *      The unencoded/decoded string.
     */
    std::string decode(const std::string& encoded_string, bool has_padding=false); 
    
    /**
     * Encode the given URL string into Base64 URL string.
     * 
     * @param[in] unencoded_uri_string
     *      The unencoded URL string.
     * @return std::string
     *      The encoded URL string.
     */
    std::string encode_url(const std::string& unencoded_uri_string);
    
    /**
     * Decode the given Base64 encoded string.
     * 
     * @param[in] encoded_uri_string
     *      The encoded URL string.
     * @return std::string
     *      The decoded URL string.
     */
    std::string decode_url(const std::string& encoded_uri_string);
}
    
#endif