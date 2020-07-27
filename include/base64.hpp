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
    std::string encode(const std::string& unencoded_string) const;
    std::string decode(const std::string& encoded_string) const; 
};

#endif