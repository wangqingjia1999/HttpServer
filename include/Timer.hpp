#pragma once

#include <ctime>
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
     * Get current time.
     * 
     * @return  
     *      HTTP Date string.
     * 
     * @example
     *      Tue, 15 Nov 1994 08:12:31 GMT
     *      Wed, 26 Aug 2020 14:17:49 GMT
     * 
     * @note
     *      Total charaters are 30 (include tailing '\0')
     */
    static std::string get_current_http_time();

    /**
     * Get elapsed time in millisecond ( 1/1000s ).
     * 
     * @return  
     *      Duration in milliseconds.
     * 
     * @note  
     *      Call reset_start_time() first to set starting time, 
     *      then call this function :)
     */
    milisecond_duration_type get_elapsed_time() const;

    /**
     * Reset start_time time point.
     */
    void reset_start_time();
    
private:
    clock_type::time_point start_time;
 
};

inline std::string Timer::get_current_http_time()
{
    time_t now = std::time(0);
    tm* current_time = std::gmtime(&now);
    char current_time_string[30];
    std::strftime(current_time_string, 30, "%a, %d %b %Y %H:%M:%S GMT", current_time);
    return current_time_string;
}

inline void Timer::reset_start_time()
{
    start_time = clock_type::now();
}

inline Timer::milisecond_duration_type Timer::get_elapsed_time() const
{
    return (clock_type::now() - start_time);
}

/**
 * Get current time in the form of [2021-01-31 20:05:58.906]
 * 
 * @return
 *      Time string.
 */
inline std::string get_current_time()
{
    std::time_t now = std::time(0);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", std::gmtime(&now));
    return buffer;
}

/**
 * Get date in the form of 2021-02-10
 * 
 * @return 
 *      Date string.
 */
inline std::string get_date()
{
    std::time_t now = std::time(0);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::gmtime(&now));
    return buffer;
}
