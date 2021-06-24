#include "Logger.hpp"

#include <chrono>
#include <iostream>
#include <gtest/gtest.h>

TEST(logger_tests, get_log_directory_path)
{
    EXPECT_EQ(Logger::get_log_directory_path(), "");
    Logger::info("Now, the log directory path has been initialized.");
    EXPECT_EQ(Logger::get_log_directory_path(), "/home/HttpServer/logs/");
}

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
    std::string error_message = "OoO! error happens";
    Logger::error(error_message);
}

TEST(logger_test, log_debug_message_test)
{
    std::string debug_message = "Here is the debug info";
    Logger::debug(debug_message);
}