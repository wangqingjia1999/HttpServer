#include "ResourceHandler.hpp"
#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

#include <string>
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