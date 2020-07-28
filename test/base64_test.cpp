#include <gtest/gtest.h>
#include "base64.hpp"
#include <vector>

TEST(base64_tests, encode_and_decode_test)
{
    base64 base64;

    std::string decoded_string = "A journey of a thousand miles begins with a single step";
    std::string encoded_string = "QSBqb3VybmV5IG9mIGEgdGhvdXNhbmQgbWlsZXMgYmVnaW5zIHdpdGggYSBzaW5nbGUgc3RlcA==";
    ASSERT_EQ(base64.encode(decoded_string, true), encoded_string);
    ASSERT_EQ(base64.decode(encoded_string, true), decoded_string);

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
        EXPECT_EQ(base64.encode(test_vector.decoded_string, true), test_vector.encoded_string) << "Error at index: " << index;
        EXPECT_EQ(base64.decode(test_vector.encoded_string, true), test_vector.decoded_string) << "Error at index: " << index;
        ++index;
    }
}

TEST(base64_tests, encode_and_decode_url_test)
{
    // These test vectors were taken from
    // [RFC 4648](https://tools.ietf.org/html/rfc4648).
    struct TestVector 
    {
        std::string decoding;
        std::string encoding;
    };
    const std::vector< TestVector > testVectors{
        {"", ""},
        {"\xff", "_w"},
        {"f\xff", "Zv8"},
        {"fo\xff", "Zm__"},
        {"foo\xff", "Zm9v_w"},
        {"foob\xff", "Zm9vYv8"},
        {"fooba\xff", "Zm9vYmH_"},
        {"foobar\xff", "Zm9vYmFy_w"},
    };

    base64 base64;
    size_t index = 0;
    for (const auto& testVector: testVectors) 
    {
        EXPECT_EQ( testVector.encoding, base64.encode_url(testVector.decoding)) << "Error at index: " << index;
        EXPECT_EQ( testVector.decoding, base64.decode_url(testVector.encoding)) << "Error at index: " << index;
        ++index;
    }
}
