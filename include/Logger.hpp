#pragma once

#include "Timer.hpp"
#include "ServerConfiguration.hpp"

#include <cstring>
#include <cerrno>

#include <chrono>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace 
{
    static std::string m_log_directory_path = {};

    enum class LogLevel {
        INFO,
        WARN,
        ERROR,
        DEBUG
    };

    /**
     * Log a message based on given level and messsage.
     * 
     * @param[in] log_level  
     *      Denote the logging level/urgency/priority.
     * 
     * @param[in] log_message
     *      Message to be logged.
     * 
     * @param[in] log_errno
     *      Optional. Interpret system errno if any.
     */
    inline void log(const LogLevel& log_level, const std::string& log_message, const int log_errno)
    {
        if(m_log_directory_path.empty())
        {
            m_log_directory_path = ServerConfiguration::instance()->get_log_directory_path();
        }
        
        std::ofstream log_file(
            m_log_directory_path + get_date() + ".log", 
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
        
        log_file << log_message;
        
        if(log_errno != 0)
            log_file << " : " << std::string{ strerror(errno) };

        log_file << '\n';

        log_file.close();
    }   
}

namespace Logger
{
    
    /**
     * Log normal intformational message.
     * 
     * @param[in] normal_message  
     *      Normal info message.
     */
    inline void info(const std::string& info_message, int log_errno = 0)
    {
        log(LogLevel::INFO, info_message, log_errno);
    }

    /**
     * Log warning message.
     * 
     * @param[in] warning_message  
     *      Warning message.
     */
    inline void warn(const std::string& warning_message, int log_errno = 0)
    {
        log(LogLevel::WARN, warning_message, log_errno);
    }

    /**
     * Log error message.
     * 
     * @param[in] error_message  
     *      Error message.
     */
    inline void error(const std::string& error_message, int log_errno = 0)
    {
        log(LogLevel::ERROR, error_message, log_errno);
    }

    /**
     * Log debug message.
     */
    inline void debug(const std::string& debug_message, int log_errno = 0)
    {
        log(LogLevel::DEBUG, debug_message, log_errno);
    }

    /**
     * Get log directory path.
     * 
     * @return 
     *      Log directory path if any.
     */
    inline std::string get_log_directory_path()
    {
        if(m_log_directory_path.empty())
        {
            m_log_directory_path = ServerConfiguration::instance()->get_log_directory_path();
        }
        return m_log_directory_path;
    }
}
