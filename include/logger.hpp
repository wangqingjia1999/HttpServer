#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>

namespace logger
{
    /**
     * @brief  Record a message into log.txt.
     * @param  message_string  Log information to be stored into log.txt.
     * 
     * @note  Store the logs into "../HttpServer/log.txt".
     */
    void record(const std::string& message_string);
}

#endif