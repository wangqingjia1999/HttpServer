#ifndef BASE64_HPP
#define BASE64_HPP
#include <memory>

class base64
{
public:
    ~base64() noexcept;
    base64();
    base64(const base64&) = delete;
    base64& operator=(const base64&) = delete;
    base64(base64&&) noexcept = delete;
    base64& operator=(base64&&) noexcept = delete;
public:
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
    std::string encode(const std::string& unencoded_string, bool has_padding=false) const;

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
    std::string decode(const std::string& encoded_string, bool has_padding=false) const; 
    
    /**
     * Encode the given URL string into Base64 URL string.
     * 
     * @param[in] unencoded_uri_string
     *      The unencoded URL string.
     * @return std::string
     *      The encoded URL string.
     */
    std::string encode_url(const std::string& unencoded_uri_string) const;
    
    /**
     * Decode the given Base64 encoded string.
     * 
     * @param[in] encoded_uri_string
     *      The encoded URL string.
     * @return std::string
     *      The decoded URL string.
     */
    std::string decode_url(const std::string& encoded_uri_string) const;
};

#endif