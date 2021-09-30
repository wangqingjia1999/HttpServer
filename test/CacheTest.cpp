#include "Cache.hpp"

#include <gtest/gtest.h>

TEST(cache_tests, cache_test)
{
    HTTP::Cache cache;

    std::string key = "/home/bitate/?q=";
    std::string value = "demo";
    EXPECT_TRUE(cache.insert(key, value));
    EXPECT_EQ(cache.get(key), value);
    EXPECT_TRUE(cache.erase(key));
}