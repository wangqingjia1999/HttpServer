#include <gtest/gtest.h>
#include "base64.hpp"
#include <vector>

TEST(base64_tests, encode_test)
{
    base64 base64;

    std::string decoded_string = "A journey of a thousand miles begins with a single step";
    std::string encoded_string = "QSBqb3VybmV5IG9mIGEgdGhvdXNhbmQgbWlsZXMgYmVnaW5zIHdpdGggYSBzaW5nbGUgc3RlcA==";
    ASSERT_EQ(base64.encode(decoded_string), encoded_string);

    struct test_vector
    {
        std::string decoded_string;
        std::string encoded_string;
    };
    const std::vector< test_vector > test_vectors{
        {"", ""},
        {"f", "Zg=="},
        {"fo", "Zm8="},
        {"foo", "Zm9v"},
        {"foob", "Zm9vYg=="},
        {"fooba", "Zm9vYmE="},
        {"foobar", "Zm9vYmFy"},
    };

    size_t index = 0;
    for(auto& test_vector : test_vectors)
    {
        EXPECT_EQ(base64.encode(test_vector.decoded_string), test_vector.encoded_string) << "Error at index: " << index;
        ++index;
    }
}

TEST(base64_tests, decode_test)
{
    base64 base64;

    std::string decoded_string = "A journey of a thousand miles begins with a single step";
    std::string encoded_string = "QSBqb3VybmV5IG9mIGEgdGhvdXNhbmQgbWlsZXMgYmVnaW5zIHdpdGggYSBzaW5nbGUgc3RlcA==";
    ASSERT_EQ(base64.decode(encoded_string), decoded_string);

    struct test_vector
    {
        std::string decoded_string;
        std::string encoded_string;
    };
    const std::vector< test_vector > test_vectors{
        {"", ""},
        {"f", "Zg=="},
        {"fo", "Zm8="},
        {"foo", "Zm9v"},
        {"foob", "Zm9vYg=="},
        {"fooba", "Zm9vYmE="},
        {"foobar", "Zm9vYmFy"},
    };

    size_t index = 0;
    for(auto& test_vector : test_vectors)
    {
        EXPECT_EQ(base64.decode(test_vector.encoded_string), test_vector.decoded_string) << "Error at index: " << index;
        ++index;
    }
}