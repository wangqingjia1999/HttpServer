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
        "# Type your server's root directory path below\n"
        "root_directory_path: /home/bitate/HttpServer/\n"
        "# Type your server's resources directory path below\n"
        "resource_directory_path: /home/bitate/HttpServer/resource/\n"
    };

    config.parse_configuration(configuration);
    EXPECT_EQ(config.get_root_directory_path(), "/home/bitate/HttpServer/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/bitate/HttpServer/resource/");
}

TEST(server_configuration_tests, parse_configuration_file_test_1)
{
    ServerConfiguration config;
    
    std::string configuration = {
        "# Type your server's root directory path below\n"
        "root_directory_path: /home/bitate/HttpServer-this-is-a-very-very-very-very-long-path/\n"
        "# Type your server's resources directory path below\n"
        "resource_directory_path: /home/bitate/HttpServer/resource/wkkxljl20934j-xcvlkjk13-sldkfj\n"
    };

    config.parse_configuration(configuration);
    EXPECT_EQ(config.get_root_directory_path(), "/home/bitate/HttpServer-this-is-a-very-very-very-very-long-path/");
    EXPECT_EQ(config.get_resource_directory_path(), "/home/bitate/HttpServer/resource/wkkxljl20934j-xcvlkjk13-sldkfj");
}
