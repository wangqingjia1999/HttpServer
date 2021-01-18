#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

TEST(server_configuration_tests, check_whether_configuration_file_exists)
{
    ServerConfiguration config;
    EXPECT_TRUE(config.has_configuration_file());
}