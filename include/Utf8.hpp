#ifndef UTF8_HPP
#define UTF8_HPP

/**
 * @file Utf8.hpp
 *
 * This module declares the Utf8::Utf8 class.
 *
 * © 2018 by Richard Walters
 */

#include <memory>
#include <stdint.h>
#include <string>
#include <ostream>
#include <vector>

namespace Utf8 {

    /**
     * This represents a single character in Unicode.
     */
    typedef uint32_t UnicodeCodePoint;

    /**
     * @brief  converting ASCII string to its equivalent sequence of Unicode code points.
     * @param  ascii  ASCII string.
     * @return  The Unicode code points sequence.
     */
    std::vector< UnicodeCodePoint > AsciiToUnicode(const std::string& ascii);

    /**
     * @brief  This class is used to encode or decode Unicode "code points",
     * or characters from many different international character sets,
     * in order to store or transmit them across any interface
     * that accepts a sequence of bytes.
     */
    class Utf8 {
        // Lifecycle management
    public:
        ~Utf8() noexcept;
        Utf8();
        Utf8(const Utf8&) = delete;
        Utf8(Utf8&&) noexcept = delete;
        Utf8& operator=(const Utf8&) = delete;
        Utf8& operator=(Utf8&&) noexcept = delete;

    public:
        /**
         * @brief  Encode Unicode code points to UTF-8 sequence.
         * @param  codePoints  Unicode code points.
         * @return  The UTF-8 encoding sequence.
         */
        static std::vector< uint8_t > Encode(const std::vector< UnicodeCodePoint >& codePoints);

        /**
         * @brief  Decode UTF-8 sequence to Unicode sequence.
         * @param  encoding  UTF-8 encoded bytes sequence.
         * @return  Unicode code points sequence.
         * 
         * This function accepts the given sequence of UTF-8 encoded bytes,
         * and returns any Unicode code points formed from them.
         *
         * Any partial code sequence at the end is held onto and used first
         * when this method is called again later.
         */
        std::vector< UnicodeCodePoint > Decode(const std::vector< uint8_t >& encoding);

        /**
         * @brief  Decode UTF-8 sequence to Unicode sequnce.
         * @param  encoding  UTF-8 encoded sequence.
         * @return  Unicode code points.
         */
        std::vector< UnicodeCodePoint > Decode(const std::string& encoding);

        /**
         * @brief  Whether it is UTF-8 encoding?
         * @param  encoding  UTF-8 encoded bytes sequence.
         * @param  final  A flag indicates whether or not this is the end of the encoding.  
         * @return  True if it is UTF-8 encoding.
         * 
         * If the final is false, any partial code sequence at the end 
         * is held onto and used first when this method is called again later.
         */
        bool IsValidEncoding(
            const std::string& encoding,
            bool final = true
        );

        // Private properties
    private:
        /**
         * @brief  This is the type of structure that contains the private
         * properties of the instance.  It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
        struct Impl;

        /**
         * @brief  This contains the private properties of the instance.
         */
        std::unique_ptr< Impl > impl_;
    };

}

#endif /* UTF8_HPP */
