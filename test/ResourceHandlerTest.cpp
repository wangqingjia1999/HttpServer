#include "ResourceHandler.hpp"
#include "ServerConfiguration.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

TEST(resource_handler_tests, default_constructor_test)
{
    ResourceHandler resource_handler{};
    EXPECT_EQ(resource_handler.get_resource_directory_path(), "/home/word-finder/resource/");
}

TEST(resource_handler_tests, formalize_resource_path_test)
{
    ResourceHandler resource_handler;

    struct TestCase
    {
        std::string input_path;
        std::string output_path;
    };

    std::vector<TestCase> tests{
        {"/", ""},
        {"/demo.mp3", "demo.mp3"},
        {"/audios/demo.mp3", "audios/demo.mp3"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(
            resource_handler.formalize_resource_path(test.input_path),
            test.output_path);
    }
}

TEST(resource_handler_tests, query_headlines_test)
{
    std::string request = {
        "GET /search?q=fly HTTP/1.1\r\n"
        "\r\n"};

    ResourceHandler resource_handler;
    std::shared_ptr<Connection> connection = std::make_shared<Connection>();
    connection->get_request()->set_raw_request(request);
    EXPECT_TRUE(connection->get_request()->parse_raw_request());
    EXPECT_TRUE(resource_handler.fetch_resource(connection));
}