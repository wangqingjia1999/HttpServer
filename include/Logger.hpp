#pragma once

#include <memory>
#include <string>

class Logger
{
    enum class Logger_Level {
        NORMAL,  
        WARNING, 
        ERROR    
    };

public:
    /**
     * Record normal intformational message.
     * 
     * @param[in] normal_message  
     *      Normal info message.
     */
    void record_normal_message(const std::string& normal_message);

    /**
     * Record warning message.
     * 
     * @param[in] warning_message  
     *      Warning message.
     */
    void record_warning_message(const std::string& warning_message);

    /**
     * Record errors.
     * 
     * @param[in] error_message  
     *      Error message.
     */
    void record_error_message(const std::string& error_message);

    /**
     * Clear up log.
     */
    void clear_up_log();
    
    /**
     * Get log.txt's absolute path.
     * 
     * @return  
     *      The absolute path string.
     */
    std::string get_log_file_absolute_path();

    /**
     * Return the first line of log.txt.
     * 
     * @return  
     *      The first line of log.txt.
     */
    std::string get_first_line_of_log();

private:
    /**
     * Record a message based on given logging level.
     * 
     * @param[in] message_string  
     *      Log information to be stored into log.txt.
     * 
     * @param[in] logger_level  
     *      Denote the logging level/urgency/priority.
     * 
     * @note  
     *      Store the log file into "../HttpServer/log.txt".
     */
    void record(
        const std::string& message_string, 
        const Logger_Level logger_level = Logger_Level::NORMAL
    );
};
