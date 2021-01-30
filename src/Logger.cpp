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



