#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>
#include <chrono>

class Timer
{
public:
    using milliseconds_type = std::chrono::milliseconds;
    using seconds_type = std::chrono::seconds;
    using clock_type = std::chrono::high_resolution_clock;
    using duration_type = clock_type::duration;

public:
    /**
     * @brief  Get current time.
     * @return  HTTP Date string.
     * 
     * For example: 
     * Tue, 15 Nov 1994 08:12:31 GMT
     * Wed, 26 Aug 2020 14:17:49 GMT
     * 
     * Total charaters are 30 (include tailing '\0')
     */
    static std::string get_current_http_time();

    duration_type get_elapsed_time_in_millisecond() const;
    
    duration_type get_elapsed_time_in_second() const;

    void reset_start_time();

private:
    duration_type get_elapsed_time() const;

private:
    clock_type::time_point start_time;
 
};
#endif

