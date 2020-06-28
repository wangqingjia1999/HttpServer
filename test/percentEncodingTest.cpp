#include <gtest/gtest.h>
#include "PercentEncoding.hpp"

#include <stddef.h>
#include <vector>

TEST(DecodeTests, ASCIICharacterSet)
{
	Uri::PercentEncoding percentEncode;

	std::string decodedString = "https://tools.ietf.org/html/rfc3986#section-2";
	std::string encodedString = "https%3A%2F%2Ftools.ietf.org%2Fhtml%2Frfc3986%23section-2";
	ASSERT_EQ(percentEncode.Decode(encodedString), decodedString);

	std::string decodedString1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	std::string encodedString1 = "ldap%3A%2F%2F%5B2001%3Adb8%3A%3A7%5D%2Fc%3DGB%3FobjectClass%3Fone";
	ASSERT_EQ(percentEncode.Decode(encodedString1), decodedString1);

	std::string decodedString2 = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	std::string encodedString2 = "urn%3Aoasis%3Anames%3Aspecification%3Adocbook%3Adtd%3Axml%3A4.1.2";
	ASSERT_EQ(percentEncode.Decode(encodedString2), decodedString2);

	std::string decodedString3 = "mailto:John.Doe@example.com";
	std::string encodedString3 = "mailto%3AJohn.Doe%40example.com";
	ASSERT_EQ(percentEncode.Decode(encodedString3), decodedString3);

	std::string decodedString4 = "tel:+1-816-555-1212";
	std::string encodedString4 = "tel%3A%2B1-816-555-1212";
	ASSERT_EQ(percentEncode.Decode(encodedString4), decodedString4);
}

TEST(EncodeTests, ASCIICharacterSet)
{
	Uri::PercentEncoding percentEncode;

	std::string decodedString = "https://tools.ietf.org/html/rfc3986#section-2";
	std::string encodedString = "https%3A%2F%2Ftools.ietf.org%2Fhtml%2Frfc3986%23section-2";
	ASSERT_EQ(percentEncode.Encode(decodedString), encodedString);

	std::string decodedString1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	std::string encodedString1 = "ldap%3A%2F%2F%5B2001%3Adb8%3A%3A7%5D%2Fc%3DGB%3FobjectClass%3Fone";
	ASSERT_EQ(percentEncode.Encode(decodedString1), encodedString1);

	std::string decodedString2 = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	std::string encodedString2 = "urn%3Aoasis%3Anames%3Aspecification%3Adocbook%3Adtd%3Axml%3A4.1.2";
	ASSERT_EQ(percentEncode.Encode(decodedString2), encodedString2);

	std::string decodedString3 = "mailto:John.Doe@example.com";
	std::string encodedString3 = "mailto%3AJohn.Doe%40example.com";
	ASSERT_EQ(percentEncode.Encode(decodedString3), encodedString3);

	std::string decodedString4 = "tel:+1-816-555-1212";
	std::string encodedString4 = "tel%3A%2B1-816-555-1212";
	ASSERT_EQ(percentEncode.Encode(decodedString4), encodedString4);
}

//TEST(PercentDecoding, DecodeLowerHexoCharacters)
//{
//
//}

//TEST(PercentEncoding, UTF8)
//{
//
//}

//TEST(PercentEncodingTests, GoodSequences) 
//{
//    Uri::PercentEncoding pec;
//    struct TestVector 
//    {
//        char sequence[2];
//        char expectedOutput;
//    };
//    const std::vector< TestVector > testVectors
//    {
//        {{'4', '1'}, 'A'},
//        {{'5', 'A'}, 'Z'},
//        {{'6', 'e'}, 'n'},
//        {{'e', '1'}, (char)0xe1},
//        {{'C', 'A'}, (char)0xca},
//        {{'7', '1'}, 'q'},
//    };
//    size_t index = 0;
//    for (auto testVector : testVectors) 
//    {
//        pec = Uri::PercentEncoding();
//        ASSERT_FALSE(pec.Done());
//        ASSERT_TRUE(pec.NextEncodedCharacter(testVector.sequence[0]));
//        ASSERT_FALSE(pec.Done());
//        ASSERT_TRUE(pec.NextEncodedCharacter(testVector.sequence[1]));
//        ASSERT_TRUE(pec.Done());
//        ASSERT_EQ(testVector.expectedOutput, pec.GetDecodedCharacter()) << index;
//        ++index;
//    }
//}
//
//TEST(PercentEncodingTests, BadSequences) 
//{
//    Uri::PercentEncoding pec;
//    std::vector< char > testVectors
//    {
//        'G', 'g', '.', 'z', '-', ' ', 'V',
//    };
//    for (auto testVector : testVectors) 
//    {
//        pec = Uri::PercentEncoding();
//        ASSERT_FALSE(pec.Done());
//        ASSERT_FALSE(pec.NextEncodedCharacter(testVector));
//    }
//}


