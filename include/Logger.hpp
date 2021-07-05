#pragma once

#include "Timer.hpp"
#include "ServerConfiguration.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace Logger
{
    static std::string log_directory_path = {};

    enum class LogLevel {
        INFO,
        WARN,
        ERROR,
        DEBUG
    };

    /**
     * Log a message based on given logging level.
     * 
     * @param[in] log_level  
     *      Denote the logging level/urgency/priority.
     * 
     * @param[in] log_message
     *      Message to be logged.
     * 
     * @note  
     *      Store the log file into the folder, which 
     *      can be configured in `config` file under 
     *      server's root directory. If not specified 
     *      or not found, the default path (same path 
     *      as the executable file) is used.
     */
    inline void log(const LogLevel& log_level, const std::string& log_message)
    {
        if(log_directory_path.empty())
        {
            ServerConfiguration server_configuration;
            log_directory_path = server_configuration.get_log_directory_path();
        }
        
        std::ofstream log_file(
            log_directory_path + get_date() + ".log", 
            std::ios_base::app
        );
        if(!log_file.is_open())
            return;

        log_file << get_current_time();
        if(log_level == LogLevel::INFO)
            log_file << " [info] ";
        else if(log_level == LogLevel::WARN)
            log_file << " [warning] ";
        else if(log_level == LogLevel::ERROR)
            log_file << " [error] ";
        else
            log_file << " [debug] ";
        log_file << log_message << '\n';

        log_file.close();
    }   

    /**
     * Log normal intformational message.
     * 
     * @param[in] normal_message  
     *      Normal info message.
     */
    inline void info(const std::string& info_message)
    {
        log(LogLevel::INFO, info_message);
    }

    /**
     * Log warning message.
     * 
     * @param[in] warning_message  
     *      Warning message.
     */
    inline void warn(const std::string& warning_message)
    {
        log(LogLevel::WARN, warning_message);
    }

    /**
     * Log error message.
     * 
     * @param[in] error_message  
     *      Error message.
     */
    inline void error(const std::string& error_message)
    {
        log(LogLevel::ERROR, error_message);
    }

    /**
     * Log debug message.
     */
    inline void debug(const std::string& debug_message)
    {
        log(LogLevel::DEBUG, debug_message);
    }

    /**
     * Get log directory path.
     * 
     * @return 
     *      Log directory path if any.
     */
    inline std::string get_log_directory_path()
    {
        if(log_directory_path.empty())
        {
            ServerConfiguration server_configuration;
            log_directory_path = server_configuration.get_log_directory_path();
        }
        return log_directory_path;
    }
}
