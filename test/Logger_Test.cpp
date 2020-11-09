#include "Logger.hpp"

#include <chrono>
#include <gtest/gtest.h>

TEST(logger_tests, record_log_message_test)
{
    std::string log_message = "This is a message";
    Logger::record(log_message, Logger::Logger_Level::NORMAL);
    EXPECT_EQ( Logger::get_first_line_of_log(), log_message);
    Logger::clear_up_log();
}