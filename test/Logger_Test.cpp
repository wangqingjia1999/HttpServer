#include "Logger.hpp"

#include <chrono>
#include <gtest/gtest.h>

TEST(logger_tests, record_normal_message_test)
{
    Logger logger;
    std::string normal_message = "This is a normal message";
    std::string formatted_normal_message = "[NORMAL]: This is a normal message";
    
    logger.record_normal_message( normal_message );
    
    EXPECT_EQ( logger.get_first_line_of_log(), formatted_normal_message);
    
    logger.clear_up_log();
}

TEST(logger_tests, record_warning_message_test)
{
    Logger logger;
    std::string warning_message = "Something that you need to pay attention to";

    std::string formatted_warning_message = "[WARNING]: Something that you need to pay attention to";

    logger.record_warning_message(warning_message);
    
    EXPECT_EQ( logger.get_first_line_of_log(), formatted_warning_message);
    
    logger.clear_up_log();
}

TEST(logger_test, record_error_message_test)
{
    Logger logger;
    std::string error_message = "OoO! errors happen";

    std::string formatted_error_message = "[ERROR]: OoO! errors happen";

    logger.record_error_message(error_message);
    
    EXPECT_EQ( logger.get_first_line_of_log(), formatted_error_message);
    
    logger.clear_up_log();
}