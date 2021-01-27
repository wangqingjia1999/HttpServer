#include "Uri.hpp"

#include <gtest/gtest.h>


TEST(UriTests, parseFromUrlString)
{
	Uri uri;
	ASSERT_TRUE(uri.parse_from_string("https://www.github.com/Bitate/httpserver"));
	ASSERT_EQ(uri.get_scheme(), "https");
	ASSERT_EQ(uri.get_host(), "www.github.com");
	ASSERT_EQ(uri.get_path(), (std::vector<std::string>{"Bitate", "httpserver"}));
}
 

TEST(UriTests, parseFromStringWithPathCornerCases)
{
	struct TestVector
	{
		std::string pathIn;
		std::vector< std::string > pathOut;
	};

	const std::vector< TestVector > test_vectors
	{
		{"",{""}},
		{"/", {""}},
		{"/foo", {"foo"}},
		{"foo/", {"foo",""}},
	};

	size_t index = 0;
	for (const auto& testVector : test_vectors)
	{
		Uri uri;
		ASSERT_TRUE(uri.parse_from_string(testVector.pathIn)) << index;
		ASSERT_EQ(uri.get_path(), testVector.pathOut) << index;
		++index;
	}
}

TEST(UriTests, parseFromStringWithAPortNumber)
{
	Uri uri;
	ASSERT_TRUE(uri.parse_from_string("http://www.example.com:8080/foo/bar"));
	ASSERT_EQ(uri.get_host(), "www.example.com");
	ASSERT_TRUE(uri.has_port());
	ASSERT_EQ(uri.get_port(), 8080);
}

TEST(UriTests, parseFromStringWithoutAPortNumber)
{
	Uri uri;
	ASSERT_TRUE(uri.parse_from_string("http://www.example.com/foo/bar"));
	ASSERT_EQ(uri.get_host(), "www.example.com");
	ASSERT_FALSE(uri.has_port());
}

TEST(UriTests, parseFromStringTwiceFirstWithPortNumberThenWithout) 
{
	Uri uri, uri2;
	ASSERT_TRUE(uri.parse_from_string("http://www.example.com:8080/foo/bar"));
	ASSERT_TRUE(uri2.parse_from_string("http://www.example.com/foo/bar"));
	ASSERT_TRUE(uri.has_port());
	ASSERT_FALSE(uri2.has_port());
}

TEST(UriTests, parseFromStringWithLargestValidPortNumber) 
{
	Uri uri;
	ASSERT_TRUE(uri.parse_from_string("http://www.example.com:65535/foo/bar"));
	ASSERT_TRUE(uri.has_port());
	ASSERT_EQ(uri.get_port(), 65535);
}


TEST(UriTests, parseFromStringBadPortNumberStartsNumericEndsAlphabetic) 
{
   Uri uri;
   ASSERT_FALSE(uri.parse_from_string("http://www.example.com:8080spam/foo/bar"));
}

TEST(UriTests, parseFromStringBadPortNumberStartsAlphabeticEndsNumeric) 
{
   Uri uri;
   ASSERT_FALSE(uri.parse_from_string("http://www.example.com:spam8080/foo/bar"));
}



TEST(UriTests, parseFromStringBadPortNumberTooBig) 
{
   Uri uri;
   ASSERT_FALSE(uri.parse_from_string("http://www.example.com:65536/foo/bar"));

   ASSERT_FALSE(uri.parse_from_string("http://www.example.com:655360/foo/bar"));

   ASSERT_FALSE(uri.parse_from_string("http://www.example.com:6553600/foo/bar"));
}

TEST(UriTests, parseFromStringBadPortNumberNegative) 
{
	Uri uri;
	ASSERT_FALSE(uri.parse_from_string("http://www.example.com:-1234/foo/bar"));
	ASSERT_FALSE(uri.has_port());
}

TEST(UriTests, parseFromStringBadPortNumberNegative1) 
{
	Uri uri;
	ASSERT_FALSE(uri.parse_from_string("http://www.example.com:1-234/foo/bar"));
	ASSERT_FALSE(uri.has_port());
}

TEST(UriTests, parseFromStringBadPortNumberNegative2) 
{
	Uri uri;
	ASSERT_FALSE(uri.parse_from_string("http://www.example.com:1234-/foo/bar"));
	ASSERT_FALSE(uri.has_port());
}

TEST(UriTests, parseFromStringEndsAfterAuthority) 
{
   Uri uri;
   ASSERT_TRUE(uri.parse_from_string("http://www.example.com"));
}

TEST(UriTests, parseFromStringRelativeVsAbsolutePath) 
{
   struct TestVector 
   {
       std::string uri;
       bool is_relative_reference;
   };
   const std::vector< TestVector > test_vectors
   {
       {"http://www.example.com/", false},
       {"http://www.example.com", false},
       {"/", true},
       {"foo", true},
   };
   size_t index = 0;
   for (const auto& testVector : test_vectors) 
   {
       Uri uri;
       ASSERT_TRUE(uri.parse_from_string(testVector.uri)) << index;
       ASSERT_EQ(testVector.is_relative_reference, uri.is_relative_reference()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringRelativeVsNonRelativePaths) 
{
   struct TestVector
   {
       std::string uri;
       bool has_relative_path;
   };
   const std::vector< TestVector > test_vectors
   {
       // Uri string               // has_relative_path
       { "http://www.example.com/",   false },
       // BUG:
       { "http://www.example.com",    false },
       { "/",                         false },
       { "foo",                       true  },
       //An empty Uri string IS a valid "relative reference" with an empty path.
       { ""                         , true },
   };
   size_t index = 0;
   for (const auto& testVector : test_vectors) 
   {
       Uri uri;
       ASSERT_TRUE(uri.parse_from_string(testVector.uri)) << index;
       ASSERT_EQ(testVector.has_relative_path, uri.has_relative_path()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringQueryAndFragmentElements) 
{
   struct TestVector 
   {
       std::string uri;
       std::string host;
       std::string query;
       std::string fragment;
   };
   const std::vector< TestVector > test_vectors
   {
	   // uriStrings                             // hosts           // query     // fragment
       {"http://www.example.com/",               "www.example.com",  "",          ""},
       {"http://example.com?foo",                "example.com",      "foo",       ""},
       {"http://www.example.com#foo",            "www.example.com",  "",          "foo"},
       {"http://www.example.com?foo#bar",        "www.example.com",  "foo",       "bar"},
       {"http://www.example.com?earth?day#bar",  "www.example.com",  "earth?day", "bar"},
       {"http://www.example.com/spam?foo#bar",   "www.example.com",  "foo",       "bar"},

       /*
        * NOTE: curiously, but we think this is correct, that
        * having a trailing question mark is equivalent to not having
        * any question mark, because in both cases, the query element
        * is empty string.  Perhaps research deeper to see if this is right.
        */
       {"http://www.example.com/?",              "www.example.com",  "",          ""},
   };
   size_t index = 0;
   for (const auto& testVector : test_vectors) 
   {
       Uri uri;
       ASSERT_TRUE(uri.parse_from_string(testVector.uri)) << index;
       ASSERT_EQ(testVector.host, uri.get_host()) << index;
       ASSERT_EQ(testVector.query, uri.get_query()) << index;
       ASSERT_EQ(testVector.fragment, uri.get_fragment()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringUserInfo) 
{
   struct TestVector 
   {
       std::string uri;
       std::string user_info;
   };
   const std::vector< TestVector > test_vectors
   {
       {"http://www.example.com/", ""},
       {"http://joe@www.example.com", "joe"},
       {"http://pepe:feelsbadman@www.example.com", "pepe:feelsbadman"},
       {"//www.example.com", ""},
       {"//bob@www.example.com", "bob"},
       {"/", ""},
       {"foo", ""},
   };
   size_t index = 0;
   for (const auto& testVector : test_vectors) 
   {
       Uri uri;
       ASSERT_TRUE(uri.parse_from_string(testVector.uri)) << index;
       ASSERT_EQ(testVector.user_info, uri.get_user_info()) << index;
       ++index;
   }
}