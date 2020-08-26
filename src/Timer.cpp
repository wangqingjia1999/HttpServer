#include "Timer.hpp"

#include <chrono>
#include <vector>

namespace
{
    std::vector< std::string > week = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    std::vector< std::string > months = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
}

namespace Timer
{
    

    std::string get_current_time()
    {
        /**
         * TODO:  
         * Hot to generate appropriate GMT time string.
         * Especially, 9 => 09 
         */

        return {};
    }
}