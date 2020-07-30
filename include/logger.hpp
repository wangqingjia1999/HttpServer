#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>

namespace logger
{
    /**
     * Store log into ".../HttpServer/log.txt" in the project root directory.
     * 
     * @param[in] message_string.
     *      Log information to be stored into log.txt.
     */
    void record(const std::string& message_string);
}

#endif