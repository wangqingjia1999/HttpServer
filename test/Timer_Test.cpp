#include "Timer.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(timer_tests, get_current_GMT_time)
{
    /**
     * HTTP Date string length is 29 characters
     * (not including tailing '\0' in C string)
     */
    ASSERT_EQ( Timer::get_current_time().size(), 29);
    std::cout << "Current GMT time is: " << std::endl;
    std::cout << Timer::get_current_time() << std::endl;
}