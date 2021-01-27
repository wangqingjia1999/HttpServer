#include "ResourceHandler.hpp"
#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

namespace
{
    ServerConfiguration server_configuration;
}

TEST(resource_handler_tests, resource_info_test)
{
    ResourceHandler resource_handler{ server_configuration.get_resource_directory_path() };
    EXPECT_TRUE(resource_handler.get_resource_directory_path().find("resource") != std::string::npos);
    std::cout << "Resource directory path is: " << resource_handler.get_resource_directory_path() << std::endl;
}

TEST(resource_handler_tests, formalize_resource_path_test)
{
    ResourceHandler resource_handler;

    struct TestCase
    {
        std::string input_path;
        std::string output_path;
    };

    std::vector< TestCase > tests
    {
        { "/",                ""                },
        { "/demo.mp3",        "demo.mp3"        },
        { "/audios/demo.mp3", "audios/demo.mp3" }
    };

    for(const auto& test : tests)
    {
        EXPECT_EQ(
            resource_handler.formalize_resource_path(test.input_path), 
            test.output_path
        );
    }
}