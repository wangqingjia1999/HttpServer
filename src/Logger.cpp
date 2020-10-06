#include "Logger.hpp"
#include "Timer.hpp"

#include <chrono>
#include <string>
#include <fstream>

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <direct.h>
#endif

namespace
{
    time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
}

namespace Logger
{
    void record(const std::string& message_string)
    {
        std::ofstream log_file_object(get_log_file_absolute_path().c_str(), std::ios::app);
        log_file_object << message_string << '\n';
    }

    void record_error(const std::string& error_message)
    {
        record("Error: can not " + error_message);
    }

    void clear_up_log()
    {
        std::ofstream output;
        output.open(get_log_file_absolute_path().c_str(), std::ofstream::out | std::ofstream::trunc);
        output.close();
    }

    std::string get_log_file_absolute_path()
    {
        // Here path_buffer will contain the current working directory path (e.g. /home/bitate/HttpServer )
        char path_buffer[1024];
#ifdef __linux__
        if(getcwd(path_buffer, 1024) == nullptr)
#elif _WIN32
        if(_getcwd(path_buffer, 1024) == nullptr)
#endif
        {
            return {};
        }

        std::string current_working_directory_string = path_buffer;
        
        // Generate absolute path to log.txt ( e.g. /home/bitate/HttpServer/log.txt )
        std::string log_file_absolute_path = current_working_directory_string + "/log.txt";

        return log_file_absolute_path;
    }

    std::string get_first_line_of_log()
    {
        std::ifstream file(get_log_file_absolute_path().c_str());

        std::string first_line;

        std::getline(file, first_line);

        return first_line;
    }

}


