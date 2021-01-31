#include "Logger.hpp"

#include <chrono>
#include <iostream>
#include <gtest/gtest.h>

TEST(logger_tests, log_info_message_test)
{
    std::string info_message = "This is a normal message";
    Logger::info(info_message);
}

TEST(logger_tests, log_warning_message_test)
{
    std::string warning_message = "Something that you need to pay attention to";
    Logger::warn(warning_message);
}

TEST(logger_test, log_error_message_test)
{
    std::string error_message = "OoO! errors happen";
    Logger::error(error_message);
}

TEST(logger_test, log_debug_message_test)
{
    std::string debug_message = "Here is the debug info";
    Logger::debug(debug_message);
}   