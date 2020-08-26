#include "Timer.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(timer_tests, get_current_GMT_time)
{
    std::string date = "Date: Tue, 15 Nov 1994 08:12:31 GMT";
    std::cout << Timer::get_current_time() << std::endl;
}