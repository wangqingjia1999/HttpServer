#include "Timer.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(timer_tests, get_current_GMT_time)
{
    /**
     * HTTP date string  is exactly 29 characters in length.
     * (not including tailing '\0' in C string)
     */
    ASSERT_EQ( Timer::get_current_http_time().size(), 29);
}