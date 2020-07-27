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
     * @return std::string
     *      The encoded Base64 string.
     */
    std::string encode(const std::string& unencoded_string) const;

    /**
     * Decode the given Base64 string.
     * 
     * @param[in] encoded_string
     *      Base64 string.
     * @return std::string
     *      The unencoded/decoded string.
     */
    std::string decode(const std::string& encoded_string) const; 
    
    /**
     * Encode the given URI string into Base64 URI string.
     * 
     * @param[in] unencoded_uri_string
     *      The unencoded URI string.
     * @return std::string
     *      The encoded URI string.
     */
    std::string encode_uri(const std::string& unencoded_uri_string) const;
    
    /**
     * Decode the given Base64 encoded string.
     * 
     * @param[in] encoded_uri_string
     *      The encoded URI string.
     * @return std::string
     *      The decoded URI string.
     */
    std::string decode_uri(const std::string& encoded_uri_string) const;
};

#endif