#include "Logger.hpp"
#include "Timer.hpp"

#include <chrono>
#include <string>
#include <fstream>

#ifdef __linux__
    #include <unistd.h>
#endif

namespace
{
    time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
}

namespace Logger
{
    void record(const std::string& message_string)
    {
        #ifdef __linux__
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
        log_file_object << message_string << '\n';
        #elif _WIN32

        #endif
    }

    void record_error(const std::string& error_message)
    {
        record("Error: can not " + error_message);
    }
}
