#include "Base64.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(base64_tests, encode_and_decode_test)
{
    std::string decoded_string = "A journey of a thousand miles begins with a single step";
    std::string encoded_string = "QSBqb3VybmV5IG9mIGEgdGhvdXNhbmQgbWlsZXMgYmVnaW5zIHdpdGggYSBzaW5nbGUgc3RlcA==";
    ASSERT_EQ(Base64::encode(decoded_string, true), encoded_string);
    ASSERT_EQ(Base64::decode(encoded_string, true), decoded_string);

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
        EXPECT_EQ(Base64::encode(test_vector.decoded_string, true), test_vector.encoded_string) << "Error at index: " << index;
        EXPECT_EQ(Base64::decode(test_vector.encoded_string, true), test_vector.decoded_string) << "Error at index: " << index;
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

    size_t index = 0;
    for (const auto& testVector: testVectors) 
    {
        EXPECT_EQ( testVector.encoding, Base64::encode_url(testVector.decoding, false)) << "Error at index: " << index;
        EXPECT_EQ( testVector.decoding, Base64::decode_url(testVector.encoding, false)) << "Error at index: " << index;
        ++index;
    }
}

TEST(base64_tests, encode_hex_string_test)
{
    /**
     * The problem is that: 
     * b37a4f2cc0624f1690f64606cf385945b2bec4ea is byte sequence in the form of hex,
     * we need convert this kind of string to byte sequence first;
     * otherwise, the Base64 encoding is wrong.
     */
    std::string hex_string = "b37a4f2cc0624f1690f64606cf385945b2bec4ea";
    std::string expected_string = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    
    ASSERT_EQ(Base64::encode_hex_string(hex_string,true), expected_string);
}