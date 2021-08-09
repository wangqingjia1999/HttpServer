#pragma once

#include <string>

#include "zlib.h"

namespace Compressor
{
    /**
     * Compress given data string.
     * 
     * @param[in] data_string
     *      Given data string with type of std::string.
     * 
     * @return
     *      Compressed data string. Empty string if error happened.
     * 
     * @ref
     *      https://panthema.net/2007/0328-ZLibString.html
     */
    std::string compress(const std::string& data_string);
}