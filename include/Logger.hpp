#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

namespace Logger
{
    enum class Logger_Level {
        NORMAL,  
        WARNING, 
        ERROR    
    };

    /**
     * @brief  Record a message into log.txt.
     * @param  message_string  Log information to be stored into log.txt.
     * @param  logger_level  Denote the logging level/urgency/priority.
     * 
     * @note  Store the log file into "../HttpServer/log.txt".
     */
    void record(
        const std::string& message_string, 
        const Logger_Level logger_level = Logger_Level::NORMAL
    );

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