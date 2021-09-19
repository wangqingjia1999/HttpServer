#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

TEST(server_configuration_tests, default_configuration_test)
{    
    EXPECT_EQ(ServerConfiguration::instance()->get_root_directory_path(), "/home/word-finder/");
    EXPECT_EQ(ServerConfiguration::instance()->get_resource_directory_path(), "/home/word-finder/resource/");
    EXPECT_EQ(ServerConfiguration::instance()->get_log_directory_path(), "/home/word-finder/logs/");
    EXPECT_EQ(ServerConfiguration::instance()->get_database_path(), "/var/lib/word-finder/data.db");
}