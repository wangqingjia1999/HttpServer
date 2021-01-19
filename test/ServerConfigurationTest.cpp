#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

TEST(server_configuration_tests, configuration_file_existence_test)
{
    ServerConfiguration config;
    EXPECT_TRUE(config.has_configuration_file());
}

TEST(server_configuration_tests, parse_configuration_file_test)
{
    ServerConfiguration config;
    
    std::string configuration = {
        "# Type your server's root directory path below"
        "/home/bitate/HttpServer/"
        "# Type your server's resources directory path below"
        "/home/bitate/HttpServer/resource/"
    };

    EXPECT_TRUE(config.parse_configuration_file(configuration));
    EXPECT_EQ(config.get_root_directory_path(), "/home/bitate/HttpServer/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/bitate/HttpServer/resource/");
}