#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

namespace Logger
{
    /**
     * @brief  Record a message into log.txt.
     * @param  message_string  Log information to be stored into log.txt.
     * 
     * @note  Store the logs into "../HttpServer/log.txt".
     */
    void record(const std::string& message_string);

    /**
     * @brief  Record errors.
     * @param  error_message  Error message.
     */
    void record_error(const std::string& error_message);
    
    /**
     * @brief  Clear up log.
     */
    void clear_up_log();

    std::string get_log_file_absolute_path();

    std::string get_first_line_of_log();
}

#endif