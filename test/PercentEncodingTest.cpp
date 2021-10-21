#include "PercentEncoding.hpp"

#include <gtest/gtest.h>

TEST(decode_tests, decode_ascii_character_set_test)
{
	PercentEncoding percent_encode;

	std::string decoded_uri_string =
	    "https://tools.ietf.org/html/rfc3986#section-2";
	std::string encoded_string =
	    "https%3A%2F%2Ftools.ietf.org%2Fhtml%2Frfc3986%23section-2";
	ASSERT_EQ(percent_encode.decode(encoded_string), decoded_uri_string);

	std::string decodedString1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	std::string encodedString1 =
	    "ldap%3A%2F%2F%5B2001%3Adb8%3A%3A7%5D%2Fc%3DGB%3FobjectClass%3Fone";
	ASSERT_EQ(percent_encode.decode(encodedString1), decodedString1);

	std::string decodedString2 =
	    "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	std::string encodedString2 =
	    "urn%3Aoasis%3Anames%3Aspecification%3Adocbook%3Adtd%3Axml%3A4.1.2";
	ASSERT_EQ(percent_encode.decode(encodedString2), decodedString2);

	std::string decodedString3 = "mailto:John.Doe@example.com";
	std::string encodedString3 = "mailto%3AJohn.Doe%40example.com";
	ASSERT_EQ(percent_encode.decode(encodedString3), decodedString3);

	std::string decodedString4 = "tel:+1-816-555-1212";
	std::string encodedString4 = "tel%3A%2B1-816-555-1212";
	ASSERT_EQ(percent_encode.decode(encodedString4), decodedString4);
}

TEST(encode_tests, encode_ascii_character_set_test)
{
	PercentEncoding percent_encode;

	std::string decoded_uri_string =
	    "https://tools.ietf.org/html/rfc3986#section-2";
	std::string encoded_string =
	    "https%3A%2F%2Ftools.ietf.org%2Fhtml%2Frfc3986%23section-2";
	ASSERT_EQ(percent_encode.encode(decoded_uri_string), encoded_string);

	std::string decodedString1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	std::string encodedString1 =
	    "ldap%3A%2F%2F%5B2001%3Adb8%3A%3A7%5D%2Fc%3DGB%3FobjectClass%3Fone";
	ASSERT_EQ(percent_encode.encode(decodedString1), encodedString1);

	std::string decodedString2 =
	    "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	std::string encodedString2 =
	    "urn%3Aoasis%3Anames%3Aspecification%3Adocbook%3Adtd%3Axml%3A4.1.2";
	ASSERT_EQ(percent_encode.encode(decodedString2), encodedString2);

	std::string decodedString3 = "mailto:John.Doe@example.com";
	std::string encodedString3 = "mailto%3AJohn.Doe%40example.com";
	ASSERT_EQ(percent_encode.encode(decodedString3), encodedString3);

	std::string decodedString4 = "tel:+1-816-555-1212";
	std::string encodedString4 = "tel%3A%2B1-816-555-1212";
	ASSERT_EQ(percent_encode.encode(decodedString4), encodedString4);
}
