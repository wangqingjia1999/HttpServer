#include "Timer.hpp"

#include <chrono>
#include <vector>

namespace Timer
{
    std::string get_current_time()
    {
#ifdef __linux__
        time_t now = std::time(0);
        struct tm* current_time = std::gmtime(&now);
        char current_time_string[30];
        std::strftime(current_time_string, 30, "%a, %d %b %Y %H:%M:%S GMT", current_time);
        return current_time_string;
#elif _WIN32
    
        return {};
#endif
    }
}