#include <gtest/gtest.h>
#include "logger.hpp"

TEST(logger_test, time_test)
{
    std::string s = "Receive request from: 192.168.1.1:2838";
    logger::record(s);
}