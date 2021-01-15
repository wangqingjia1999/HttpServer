#include "Logger.hpp"
#include "Timer.hpp"

#include <chrono>
#include <string>
#include <fstream>

#include <unistd.h>


namespace
{
    time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
}

void Logger::record(const std::string& message_string, const Logger_Level logger_level)
{
    std::ofstream log_file_object(get_log_file_absolute_path().c_str(), std::ios::app);

    switch(logger_level)
    {
        case Logger_Level::NORMAL:
        {        
            log_file_object << "[NORMAL]: " << message_string << '\n';  
        }

        case Logger_Level::WARNING:
        {
            log_file_object << "[WARNING]: " << message_string << '\n';  
        }

        case Logger_Level::ERROR:
        {
            log_file_object << "[ERROR]: " << message_string << '\n';  
        }

    }
    
}

void Logger::record_normal_message(const std::string& normal_message)
{
    record(normal_message, Logger_Level::NORMAL);
}

void Logger::record_warning_message(const std::string& warning_message)
{
    record(warning_message, Logger_Level::WARNING);
}   

void Logger::record_error_message(const std::string& error_message)
{
    record(error_message, Logger_Level::ERROR);
}

void Logger::clear_up_log()
{
    std::ofstream output;
    output.open(get_log_file_absolute_path().c_str(), std::ofstream::out | std::ofstream::trunc);
    output.close();
}

std::string Logger::get_log_file_absolute_path()
{
    // Here path_buffer will contain the current working directory path (e.g. /home/bitate/HttpServer )
    char path_buffer[1024];
    if(getcwd(path_buffer, 1024) == nullptr)
    {
        return {};
    }

    std::string current_working_directory_string = path_buffer;
    
    // Generate absolute path to log.txt ( e.g. /home/bitate/HttpServer/log.txt )
    std::string log_file_absolute_path = current_working_directory_string + "/log.txt";

    return log_file_absolute_path;
}

std::string Logger::get_first_line_of_log()
{
    std::ifstream file(get_log_file_absolute_path().c_str());

    std::string first_line;

    std::getline(file, first_line);

    return first_line;
}



