#include "logger.hpp"

#include <chrono>
#include <string>
#include <fstream>
#include <unistd.h>

namespace
{
    time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
}

namespace logger
{
    void record(const std::string& message_string)
    {
        time_t current_time = time(0);
        // ctime() generate a date and time string but appending a '\n' at the last element
        std::string current_time_string_tmp = ctime(&current_time);
        // I just strip the last '\n' here
        std::string current_time_string = current_time_string_tmp.substr(0, current_time_string_tmp.size() - 1);

        // Here path_buffer will contain the current working directory path (e.g. /home/bitate/HttpServer )
        char path_buffer[1024];
        if(getcwd(path_buffer, 1024) == nullptr)
        {
            return ;
        }
        std::string current_working_directory_string = path_buffer;
        // Generate absolute path to log.txt ( e.g. /home/bitate/HttpServer/log.txt )
        std::string log_file_absolute_path = current_working_directory_string + "/log.txt";
        
        std::ofstream log_file_object(log_file_absolute_path, std::ios::app);
        // log_file_object << '[' << current_time_string << "]  " << message_string << '\n';
        log_file_object << message_string << '\n';
    }
}
