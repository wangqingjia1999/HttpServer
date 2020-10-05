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
    using milisecond_duration_type = std::chrono::duration< double, std::milli >;

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

    /**
     * @brief  Get elapsed time in millisecond ( 1/1000s ).
     * @return  Duration in milliseconds.
     * 
     * @note  Call reset_start_time() first to set starting time, 
     *        then call this function :)
     */
    milisecond_duration_type get_elapsed_time() const;

    /**
     * @brief  Reset start_time time point.
     */
    void reset_start_time();

private:
    

private:
    clock_type::time_point start_time;
 
};
#endif

