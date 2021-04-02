#include "Uri.hpp"

#include <gtest/gtest.h>

TEST(uri_tests, parse_from_url_string)
{
    Uri uri;
    ASSERT_TRUE(uri.parse_from_string("https://www.github.com/Bitate/httpserver"));
    ASSERT_EQ(uri.get_scheme(), "https");
    ASSERT_EQ(uri.get_host(), "www.github.com");
    ASSERT_EQ(uri.get_path(), (std::vector<std::string>{"Bitate", "httpserver"}));
    ASSERT_EQ(uri.get_path_string(), "Bitate/httpserver");
}

TEST(uri_tests, parse_from_string_with_path_corner_cases)
{
    struct TestVector
    {
        std::string input_path;
        std::vector<std::string> output_path;
    };

    const std::vector<TestVector> test_vectors{
        {"", {} },
        {"/", {""}},
        {"/foo", {"foo"}},
        {"foo/", {"foo", ""}},
    };

    Uri uri;
    size_t index = 0;
    for (const auto &test_vector : test_vectors)
    {
        ASSERT_TRUE(uri.parse_from_string(test_vector.input_path)) << index;
        ASSERT_EQ(uri.get_path(), test_vector.output_path) << index;
        ++index;
    }
}

TEST(uri_tests, parse_from_string_with_a_port_number)
{
    Uri uri;
    ASSERT_TRUE(uri.parse_from_string("http://www.example.com:8080/foo/bar"));
    ASSERT_EQ(uri.get_host(), "www.example.com");
    ASSERT_TRUE(uri.has_port());
    ASSERT_EQ(uri.get_port(), 8080);
}

TEST(uri_tests, parse_from_string_without_a_port_number)
{
    Uri uri;
    ASSERT_TRUE(uri.parse_from_string("http://www.example.com/foo/bar"));
    ASSERT_EQ(uri.get_host(), "www.example.com");
    ASSERT_FALSE(uri.has_port());
}

TEST(uri_tests, parse_from_string_twice_first_with_port_number_then_without)
{
    Uri uri, uri2;
    ASSERT_TRUE(uri.parse_from_string("http://www.example.com:8080/foo/bar"));
    ASSERT_TRUE(uri2.parse_from_string("http://www.example.com/foo/bar"));
    ASSERT_TRUE(uri.has_port());
    ASSERT_FALSE(uri2.has_port());
}

TEST(uri_tests, parse_from_string_with_largest_valid_port_number)
{
    Uri uri;
    ASSERT_TRUE(uri.parse_from_string("http://www.example.com:65535/foo/bar"));
    ASSERT_TRUE(uri.has_port());
    ASSERT_EQ(uri.get_port(), 65535);
}

TEST(uri_tests, parse_from_string_bad_port_number_starts_numeric_ends_alphabetic)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:8080spam/foo/bar"));
}

TEST(uri_tests, parse_bad_port_number_starts_alphabetic_and_ends_numeric)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:spam8080/foo/bar"));
}

TEST(uri_tests, parse_bad_port_number_too_big)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:65536/foo/bar"));

    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:655360/foo/bar"));

    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:6553600/foo/bar"));
}

TEST(uri_tests, parse_negative_port_number)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:-1234/foo/bar"));
    ASSERT_FALSE(uri.has_port());
}

TEST(uri_tests, parse_bad_port_number_1)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:1-234/foo/bar"));
    ASSERT_FALSE(uri.has_port());
}

TEST(uri_tests, parse_bad_port_number_2)
{
    Uri uri;
    ASSERT_FALSE(uri.parse_from_string("http://www.example.com:1234-/foo/bar"));
    ASSERT_FALSE(uri.has_port());
}

TEST(uri_tests, parse_uri_ending_with_authority)
{
    Uri uri;
    ASSERT_TRUE(uri.parse_from_string("http://www.example.com"));
}

TEST(uri_tests, parse_relative_path_and_absolute_path)
{
    struct TestVector
    {
        std::string uri;
        bool is_relative_reference;
    };
    const std::vector<TestVector> test_vectors{
        {"http://www.example.com/", false},
        {"http://www.example.com", false},
        {"/", true},
        {"foo", true},
    };

    Uri uri;
    size_t index = 0;
    for (const auto& test_vector : test_vectors)
    {
        ASSERT_TRUE(uri.parse_from_string(test_vector.uri)) << index;
        ASSERT_EQ(test_vector.is_relative_reference, uri.is_relative_reference()) << index;
        ++index;
    }
}

TEST(uri_tests, parse_relative_and_non_relative_path)
{
    struct TestVector
    {
        std::string uri;
        bool has_relative_path;
    };
    const std::vector<TestVector> test_vectors{
        // Uri string               // has_relative_path
        {"http://www.example.com/", false},
        {"http://www.example.com",  false},
        {"/",                       false},
        {"foo",                     true},
        //An empty Uri string IS a valid "relative reference" with an empty path.
        {"",                        true},
    };

    size_t index = 0;
    for (const auto& test_vector : test_vectors)
    {
        Uri uri;
        ASSERT_TRUE(uri.parse_from_string(test_vector.uri)) << index;
        ASSERT_EQ(test_vector.has_relative_path, uri.has_relative_path()) << index;
        ++index;
    }
}

TEST(uri_tests, parse_query_and_fragment_elements)
{
    struct TestVector
    {
        std::string uri;
        std::string host;
        std::string query;
        std::string fragment;
    };

    const std::vector<TestVector> test_vectors
    {
        // uriStrings                             // hosts           // query     // fragment
        {"http://www.example.com/",             "www.example.com",      "",          ""},
        {"http://example.com?foo",              "example.com",          "foo",       ""},
        {"http://www.example.com#foo",          "www.example.com",      "",          "foo"},
        {"http://www.example.com?foo#bar",      "www.example.com",      "foo",       "bar"},
        {"http://www.example.com?earth?day#bar","www.example.com",      "earth?day", "bar"},
        {"http://www.example.com/spam?foo#bar", "www.example.com",      "foo",       "bar"},

        /*
        * NOTE: curiously, but we think this is correct, that
        * having a trailing question mark is equivalent to not having
        * any question mark, because in both cases, the query element
        * is empty string.  Perhaps research deeper to see if this is right.
        */
        {"http://www.example.com/?", "www.example.com", "", ""},
    };

    size_t index = 0;
    for (const auto& test_vector : test_vectors)
    {
        Uri uri;
        ASSERT_TRUE(uri.parse_from_string(test_vector.uri)) << index;
        ASSERT_EQ(test_vector.host, uri.get_host()) << index;
        ASSERT_EQ(test_vector.query, uri.get_query()) << index;
        ASSERT_EQ(test_vector.fragment, uri.get_fragment()) << index;
        ++index;
    }
}

TEST(uri_tests, parse_user_info)
{
    struct TestVector
    {
        std::string uri;
        std::string user_info;
    };

    const std::vector<TestVector> test_vectors
    {
        {"http://www.example.com/", ""},
        {"http://joe@www.example.com", "joe"},
        {"http://pepe:feelsbadman@www.example.com", "pepe:feelsbadman"},
        {"//www.example.com", ""},
        {"//bob@www.example.com", "bob"},
        {"/", ""},
        {"foo", ""}
    };

    size_t index = 0;
    for (const auto& test_vector : test_vectors)
    {
        Uri uri;
        ASSERT_TRUE(uri.parse_from_string(test_vector.uri)) << index;
        ASSERT_EQ(test_vector.user_info, uri.get_user_info()) << index;
        ++index;
    }
}

TEST(uri_tests, parse_query_parameters)
{
    struct TestVector
    {
        std::string query_string;
        Uri::QueryParameters query_parameters;
    };

    std::vector<TestVector> test_vectors
    {
        { 
            "field1=value1&field2=value2&field3=value3", 
            {
                {"field1", "value1"}, 
                {"filed2", "value2"}, 
                {"filed3", "value3"} 
            } 
        },
        {
            "name=Tom&age=22&country=China",
            {
                {"name", "Tom"},
                {"age", "22"},
                {"country", "China"}
            }
        }
    };

    size_t index = 0;
    for(const auto& test_vector : test_vectors)
    {
        Uri uri;
        ASSERT_TRUE(uri.parse_query_parameters(test_vector.query_string)) << index;
        ASSERT_EQ(uri.get_query_paramters(), test_vector.query_parameters) << index;
        ++index;
    }
}