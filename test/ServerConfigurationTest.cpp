#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

TEST(server_configuration_tests, get_default_configuration_test)
{
    ServerConfiguration config;

    EXPECT_EQ(config.get_root_directory_path(), "/home/word-finder/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/word-finder/resource/");
    EXPECT_EQ(config.get_log_directory_path(), "/home/word-finder/logs/");
    EXPECT_EQ(config.get_database_path(), "/var/lib/word-finder/data.db");
}