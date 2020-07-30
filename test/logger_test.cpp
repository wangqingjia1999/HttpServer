#include <gtest/gtest.h>

#include <chrono>

#include "logger.hpp"
TEST(logger_test, time_test)
{   
    time_t current_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    std::string time_string = std::ctime(&current_time);
    std::string output = time_string + ' ' + "Receive request from: 192.168.1.1:2838";
    // check /HttpServer/log.txt for output :)
    logger::record(output);
}