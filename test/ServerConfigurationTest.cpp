#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

TEST(server_configuration_tests, configuration_file_existence_test)
{
    ServerConfiguration config;
    EXPECT_TRUE(config.has_configuration_file());
}

TEST(server_configuration_tests, parse_configuration_file_test_0)
{
    ServerConfiguration config;
    
    std::string configuration = {
        "# server's root directory path\n"
        "root_directory_path: /home/bitate/HttpServer/\n"
        "# server's resources directory path\n"
        "resource_directory_path: /home/bitate/HttpServer/resource/\n"
        "# server's logs directory path\n"
        "log_directory_path: /home/bitate/HttpServer/logs/\n"
    };

    config.parse_configuration(configuration);
    EXPECT_EQ(config.get_root_directory_path(), "/home/bitate/HttpServer/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/bitate/HttpServer/resource/");
    EXPECT_EQ(config.get_log_directory_path(), "/home/bitate/HttpServer/logs/");
}

TEST(server_configuration_tests, parse_configuration_file_test_1)
{
    ServerConfiguration config;
    
    std::string configuration = {
        "# server's root directory path\n"
        "root_directory_path: /home/bitate/HttpServer-this-is-a-very-very-very-very-long-path/\n"
        "# server's resources directory path\n"
        "resource_directory_path: /home/bitate/HttpServer/resource/wkkxljl20934j-xcvlkjk13-sldkfj\n"
    };

    config.parse_configuration(configuration);
    EXPECT_EQ(config.get_root_directory_path(), "/home/bitate/HttpServer-this-is-a-very-very-very-very-long-path/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/bitate/HttpServer/resource/wkkxljl20934j-xcvlkjk13-sldkfj");
}

TEST(server_configuration_tests, parse_database_path_test)
{
    ServerConfiguration config;

    std::string configuration = {
        "# server's database path"
        "database_path: /home/bitate/Documents/abcnews.db"
    };

    config.parse_configuration(configuration);
    EXPECT_EQ(config.get_database_path(), "/home/bitate/Documents/abcnews.db");
}