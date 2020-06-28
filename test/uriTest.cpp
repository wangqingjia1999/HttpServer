#include <gtest/gtest.h>
#include "Uri.hpp"

TEST(UriTests, parseFromUrlString)
{
	Uri::Uri uri;
	ASSERT_TRUE(uri.parseFromString("https://www.github.com/Bitate/httpserver"));
	ASSERT_EQ(uri.getScheme(), "https");
	ASSERT_EQ(uri.getHost(), "www.github.com");
	ASSERT_EQ(uri.getPath(), (std::vector<std::string>{"Bitate", "httpserver"}));
}
 

TEST(UriTests, parseFromStringWithPathCornerCases)
{
	struct TestVector
	{
		std::string pathIn;
		std::vector< std::string > pathOut;
	};

	const std::vector< TestVector > testVectors
	{
		{"",{""}},
		{"/", {""}},
		{"/foo", {"foo"}},
		{"foo/", {"foo",""}},
	};

	size_t index = 0;
	for (const auto& testVector : testVectors)
	{
		Uri::Uri uri;
		ASSERT_TRUE(uri.parseFromString(testVector.pathIn)) << index;
		ASSERT_EQ(uri.getPath(), testVector.pathOut) << index;
		++index;
	}
}

TEST(UriTests, parseFromStringWithAPortNumber)
{
	Uri::Uri uri;
	ASSERT_TRUE(uri.parseFromString("http://www.example.com:8080/foo/bar"));
	ASSERT_EQ(uri.getHost(), "www.example.com");
	ASSERT_TRUE(uri.hasPort());
	ASSERT_EQ(uri.getPort(), 8080);
}

TEST(UriTests, parseFromStringWithoutAPortNumber)
{
	Uri::Uri uri;
	ASSERT_TRUE(uri.parseFromString("http://www.example.com/foo/bar"));
	ASSERT_EQ(uri.getHost(), "www.example.com");
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, parseFromStringTwiceFirstWithPortNumberThenWithout) 
{
	Uri::Uri uri, uri2;
	ASSERT_TRUE(uri.parseFromString("http://www.example.com:8080/foo/bar"));
	ASSERT_TRUE(uri2.parseFromString("http://www.example.com/foo/bar"));
	ASSERT_TRUE(uri.hasPort());
	ASSERT_FALSE(uri2.hasPort());
}

TEST(UriTests, parseFromStringWithLargestValidPortNumber) 
{
	Uri::Uri uri;
	ASSERT_TRUE(uri.parseFromString("http://www.example.com:65535/foo/bar"));
	ASSERT_TRUE(uri.hasPort());
	ASSERT_EQ(uri.getPort(), 65535);
}


TEST(UriTests, parseFromStringBadPortNumberStartsNumericEndsAlphabetic) 
{
   Uri::Uri uri;
   ASSERT_FALSE(uri.parseFromString("http://www.example.com:8080spam/foo/bar"));
}

TEST(UriTests, parseFromStringBadPortNumberStartsAlphabeticEndsNumeric) 
{
   Uri::Uri uri;
   ASSERT_FALSE(uri.parseFromString("http://www.example.com:spam8080/foo/bar"));
}



TEST(UriTests, parseFromStringBadPortNumberTooBig) 
{
   Uri::Uri uri;
   ASSERT_FALSE(uri.parseFromString("http://www.example.com:65536/foo/bar"));

   ASSERT_FALSE(uri.parseFromString("http://www.example.com:655360/foo/bar"));

   ASSERT_FALSE(uri.parseFromString("http://www.example.com:6553600/foo/bar"));
}

TEST(UriTests, parseFromStringBadPortNumberNegative) 
{
	Uri::Uri uri;
	ASSERT_FALSE(uri.parseFromString("http://www.example.com:-1234/foo/bar"));
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, parseFromStringBadPortNumberNegative1) 
{
	Uri::Uri uri;
	ASSERT_FALSE(uri.parseFromString("http://www.example.com:1-234/foo/bar"));
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, parseFromStringBadPortNumberNegative2) 
{
	Uri::Uri uri;
	ASSERT_FALSE(uri.parseFromString("http://www.example.com:1234-/foo/bar"));
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, parseFromStringEndsAfterAuthority) 
{
   Uri::Uri uri;
   ASSERT_TRUE(uri.parseFromString("http://www.example.com"));
}

TEST(UriTests, parseFromStringRelativeVsAbsolutePath) 
{
   struct TestVector 
   {
       std::string uriString;
       bool isRelativeReference;
   };
   const std::vector< TestVector > testVectors
   {
       {"http://www.example.com/", false},
       {"http://www.example.com", false},
       {"/", true},
       {"foo", true},
   };
   size_t index = 0;
   for (const auto& testVector : testVectors) 
   {
       Uri::Uri uri;
       ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
       ASSERT_EQ(testVector.isRelativeReference, uri.isRelativeReference()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringRelativeVsNonRelativePaths) 
{
   struct TestVector
   {
       std::string uriString;
       bool hasRelativePath;
   };
   const std::vector< TestVector > testVectors
   {
       // Uri string               // hasRelativePath
       { "http://www.example.com/",   false },
       // BUG:
       { "http://www.example.com",    false },
       { "/",                         false },
       { "foo",                       true  },
       //An empty URI string IS a valid "relative reference" with an empty path.
       { ""                         , true },
   };
   size_t index = 0;
   for (const auto& testVector : testVectors) 
   {
       Uri::Uri uri;
       ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
       ASSERT_EQ(testVector.hasRelativePath, uri.hasRelativePath()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringQueryAndFragmentElements) 
{
   struct TestVector 
   {
       std::string uriString;
       std::string host;
       std::string query;
       std::string fragment;
   };
   const std::vector< TestVector > testVectors
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
   for (const auto& testVector : testVectors) 
   {
       Uri::Uri uri;
       ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
       ASSERT_EQ(testVector.host, uri.getHost()) << index;
       ASSERT_EQ(testVector.query, uri.getQuery()) << index;
       ASSERT_EQ(testVector.fragment, uri.getFragment()) << index;
       ++index;
   }
}

TEST(UriTests, parseFromStringUserInfo) 
{
   struct TestVector 
   {
       std::string uriString;
       std::string userInfo;
   };
   const std::vector< TestVector > testVectors
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
   for (const auto& testVector : testVectors) 
   {
       Uri::Uri uri;
       ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
       ASSERT_EQ(testVector.userInfo, uri.getUserInfo()) << index;
       ++index;
   }
}

// //TEST(UriTests, parseFromStringTwiceFirstUserInfoThenWithout) {
// //    Uri::Uri uri;
// //    ASSERT_TRUE(uri.parseFromString("http://joe@www.example.com/foo/bar"));
// //    ASSERT_TRUE(uri.parseFromString("/foo/bar"));
// //    ASSERT_TRUE(uri.getUserInfo().empty());
// //}
// //
// //TEST(UriTests, parseFromStringSchemeIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"://www.example.com/"},
// //        {"0://www.example.com/"},
// //        {"+://www.example.com/"},
// //        {"@://www.example.com/"},
// //        {".://www.example.com/"},
// //        {"h@://www.example.com/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringSchemeBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string scheme;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"h://www.example.com/", "h"},
// //        {"x+://www.example.com/", "x+"},
// //        {"y-://www.example.com/", "y-"},
// //        {"z.://www.example.com/", "z."},
// //        {"aa://www.example.com/", "aa"},
// //        {"a0://www.example.com/", "a0"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.scheme, uri.getScheme());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringSchemeMixedCase) {
// //    const std::vector< std::string > testVectors{
// //        {"http://www.example.com/"},
// //        {"hTtp://www.example.com/"},
// //        {"HtTP://www.example.com/"},
// //        {"HTTP://www.example.com/"},
// //        {"Http://www.example.com/"},
// //        {"HttP://www.example.com/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector)) << index;
// //        ASSERT_EQ("http", uri.getScheme()) << ">>> Failed for test vector element " << index << " <<<";
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringUserInfoIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"//%X@www.example.com/"},
// //        {"//{@www.example.com/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringUserInfoBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string userInfo;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"//%41@www.example.com/", "A"},
// //        {"//@www.example.com/", ""},
// //        {"//!@www.example.com/", "!"},
// //        {"//'@www.example.com/", "'"},
// //        {"//(@www.example.com/", "("},
// //        {"//;@www.example.com/", ";"},
// //        {"http://:@www.example.com/", ":"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.userInfo, uri.getUserInfo());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringHostIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"//%X@www.example.com/"},
// //        {"//@www:example.com/"},
// //        {"//[vX.:]/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringHostBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string host;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"//%41/", "a"},
// //        {"///", ""},
// //        {"//!/", "!"},
// //        {"//'/", "'"},
// //        {"//(/", "("},
// //        {"//;/", ";"},
// //        {"//1.2.3.4/", "1.2.3.4"},
// //        {"//[v7.:]/", "v7.:"},
// //        {"//[v7.aB]/", "v7.aB"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.host, uri.getHost());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringHostMixedCase) {
// //    const std::vector< std::string > testVectors{
// //        {"http://www.example.com/"},
// //        {"http://www.EXAMPLE.com/"},
// //        {"http://www.exAMple.com/"},
// //        {"http://www.example.cOM/"},
// //        {"http://wWw.exampLe.Com/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector)) << index;
// //        ASSERT_EQ("www.example.com", uri.getHost()) << ">>> Failed for test vector element " << index << " <<<";
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringDontMisinterpretColonInOtherPlacesAsSchemeDelimiter) {
// //    const std::vector< std::string > testVectors{
// //        {"//foo:bar@www.example.com/"},
// //        {"//www.example.com/a:b"},
// //        {"//www.example.com/foo?a:b"},
// //        {"//www.example.com/foo#a:b"},
// //        {"//[v7.:]/"},
// //        {"/:/foo"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector)) << index;
// //        ASSERT_TRUE(uri.getScheme().empty());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringPathIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"http://www.example.com/foo[bar"},
// //        {"http://www.example.com/]bar"},
// //        {"http://www.example.com/foo]"},
// //        {"http://www.example.com/["},
// //        {"http://www.example.com/abc/foo]"},
// //        {"http://www.example.com/abc/["},
// //        {"http://www.example.com/foo]/abc"},
// //        {"http://www.example.com/[/abc"},
// //        {"http://www.example.com/foo]/"},
// //        {"http://www.example.com/[/"},
// //        {"/foo[bar"},
// //        {"/]bar"},
// //        {"/foo]"},
// //        {"/["},
// //        {"/abc/foo]"},
// //        {"/abc/["},
// //        {"/foo]/abc"},
// //        {"/[/abc"},
// //        {"/foo]/"},
// //        {"/[/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringPathBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::vector< std::string > path;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"/:/foo", {"", ":", "foo"}},
// //        {"bob@/foo", {"bob@", "foo"}},
// //        {"hello!", {"hello!"}},
// //        {"//example.com/foo/(bar)/", {"", "foo", "(bar)", ""}},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.path, uri.getPath());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringQueryIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"http://www.example.com/?foo[bar"},
// //        {"http://www.example.com/?]bar"},
// //        {"http://www.example.com/?foo]"},
// //        {"http://www.example.com/?["},
// //        {"http://www.example.com/?abc/foo]"},
// //        {"http://www.example.com/?abc/["},
// //        {"http://www.example.com/?foo]/abc"},
// //        {"http://www.example.com/?[/abc"},
// //        {"http://www.example.com/?foo]/"},
// //        {"http://www.example.com/?[/"},
// //        {"?foo[bar"},
// //        {"?]bar"},
// //        {"?foo]"},
// //        {"?["},
// //        {"?abc/foo]"},
// //        {"?abc/["},
// //        {"?foo]/abc"},
// //        {"?[/abc"},
// //        {"?foo]/"},
// //        {"?[/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringQueryBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string query;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"/?:/foo", ":/foo"},
// //        {"?bob@/foo", "bob@/foo"},
// //        {"?hello!", "hello!"},
// //        {"//example.com/foo?(bar)/", "(bar)/"},
// //        {"http://www.example.com/?foo?bar", "foo?bar" },
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.query, uri.getQuery());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringFragmentIllegalCharacters) {
// //    const std::vector< std::string > testVectors{
// //        {"http://www.example.com/#foo[bar"},
// //        {"http://www.example.com/#]bar"},
// //        {"http://www.example.com/#foo]"},
// //        {"http://www.example.com/#["},
// //        {"http://www.example.com/#abc/foo]"},
// //        {"http://www.example.com/#abc/["},
// //        {"http://www.example.com/#foo]/abc"},
// //        {"http://www.example.com/#[/abc"},
// //        {"http://www.example.com/#foo]/"},
// //        {"http://www.example.com/#[/"},
// //        {"#foo[bar"},
// //        {"#]bar"},
// //        {"#foo]"},
// //        {"#["},
// //        {"#abc/foo]"},
// //        {"#abc/["},
// //        {"#foo]/abc"},
// //        {"#[/abc"},
// //        {"#foo]/"},
// //        {"#[/"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_FALSE(uri.parseFromString(testVector)) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringFragmentBarelyLegal) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string fragment;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"/#:/foo", ":/foo"},
// //        {"#bob@/foo", "bob@/foo"},
// //        {"#hello!", "hello!"},
// //        {"//example.com/foo#(bar)/", "(bar)/"},
// //        {"http://www.example.com/#foo?bar", "foo?bar" },
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.fragment, uri.getFragment());
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, parseFromStringPathsWithPercentEncodedCharacters) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string pathFirstSegment;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"%41", "A"},
// //        {"%50", "P"},
// //        {"%4A", "J"},
// //        {"%4a", "J"},
// //        {"%bc", "\xbc"},
// //        {"%Bc", "\xbc"},
// //        {"%bC", "\xbc"},
// //        {"%BC", "\xbc"},
// //        {"%41%42%43", "ABC"},
// //        {"%41%4A%43%4b", "AJCK"},
// //        {"%C3%A7", "ç"},
// //        {"%E6%88%91","我"},
// //    };
// //
// //    char wo = '我';
// //    ASSERT_EQ( wo, '\u6211');
// //    /*size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        ASSERT_EQ(testVector.pathFirstSegment, uri.getPath()[0]);
// //        ++index;
// //    }*/
// //}

// //TEST(UriTests, NormalizePath) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::vector< std::string > normalizedPathSegments;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        {"/a/b/c/./../../g", {"", "a", "g"}},
// //        {"mid/content=5/../6", {"mid", "6"}},
// //        {"http://example.com/a/../b", {"", "b"}},
// //        {"http://example.com/../b", {"", "b"}},
// //        {"http://example.com/a/../b/", {"", "b", ""}},
// //        {"http://example.com/a/../../b", {"", "b"}},
// //        {"./a/b", {"a", "b"}},
// //        {"..", {}},
// //        {"/", {""}},
// //        {"a/b/..", {"a", ""}},
// //        {"a/b/.", {"a", "b", ""}},
// //        {"a/b/./c", {"a", "b", "c"}},
// //        {"a/b/./c/", {"a", "b", "c", ""}},
// //        {"/a/b/..", {"", "a", ""}},
// //        {"/a/b/.", {"", "a", "b", ""}},
// //        {"/a/b/./c", {"", "a", "b", "c"}},
// //        {"/a/b/./c/", {"", "a", "b", "c", ""}},
// //        {"./a/b/..", {"a", ""}},
// //        {"./a/b/.", {"a", "b", ""}},
// //        {"./a/b/./c", {"a", "b", "c"}},
// //        {"./a/b/./c/", {"a", "b", "c", ""}},
// //        {"../a/b/..", {"a", ""}},
// //        {"../a/b/.", {"a", "b", ""}},
// //        {"../a/b/./c", {"a", "b", "c"}},
// //        {"../a/b/./c/", {"a", "b", "c", ""}},
// //        {"../a/b/../c", {"a", "c"}},
// //        {"../a/b/./../c/", {"a", "c", ""}},
// //        {"../a/b/./../c", {"a", "c"}},
// //        {"../a/b/./../c/", {"a", "c", ""}},
// //        {"../a/b/.././c/", {"a", "c", ""}},
// //        {"../a/b/.././c", {"a", "c"}},
// //        {"../a/b/.././c/", {"a", "c", ""}},
// //        {"/./c/d", {"", "c", "d"}},
// //        {"/../c/d", {"", "c", "d"}},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        ASSERT_TRUE(uri.parseFromString(testVector.uriString)) << index;
// //        uri.NormalizePath();
// //        ASSERT_EQ(testVector.normalizedPathSegments, uri.getPath()) << index;
// //        ++index;
// //    }
// //}
// //
// ////TEST(UriTests, ConstructNormalizeAndCompareEquivalentUris) {
// ////    // This was inspired by section 6.2.2
// ////    // of RFC 3986 (https://tools.ietf.org/html/rfc3986).
// ////    Uri::Uri uri1, uri2;
// ////    ASSERT_TRUE(uri1.parseFromString("example://a/b/c/%7Bfoo%7D"));
// ////    ASSERT_TRUE(uri2.parseFromString("eXAMPLE://a/./b/../b/%63/%7bfoo%7d"));
// ////    ASSERT_NE(uri1, uri2);
// ////    uri2.NormalizePath();
// ////    ASSERT_EQ(uri1, uri2);
// ////}
// //
// //TEST(UriTests, ReferenceResolution) {
// //    struct TestVector {
// //        std::string baseString;
// //        std::string relativeReferenceString;
// //        std::string targetString;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        // These are all taken from section 5.4.1
// //        // of RFC 3986 (https://tools.ietf.org/html/rfc3986).
// //        {"http://a/b/c/d;p?q", "g:h", "g:h"},
// //        {"http://a/b/c/d;p?q", "g", "http://a/b/c/g"},
// //        {"http://a/b/c/d;p?q", "./g", "http://a/b/c/g"},
// //        {"http://a/b/c/d;p?q", "g/", "http://a/b/c/g/"},
// //        {"http://a/b/c/d;p?q", "//g", "http://g"},
// //        {"http://a/b/c/d;p?q", "?y", "http://a/b/c/d;p?y"},
// //        {"http://a/b/c/d;p?q", "g?y", "http://a/b/c/g?y"},
// //        {"http://a/b/c/d;p?q", "#s", "http://a/b/c/d;p?q#s"},
// //        {"http://a/b/c/d;p?q", "g#s", "http://a/b/c/g#s"},
// //        {"http://a/b/c/d;p?q", "g?y#s", "http://a/b/c/g?y#s"},
// //        {"http://a/b/c/d;p?q", ";x", "http://a/b/c/;x"},
// //        {"http://a/b/c/d;p?q", "g;x", "http://a/b/c/g;x"},
// //        {"http://a/b/c/d;p?q", "g;x?y#s", "http://a/b/c/g;x?y#s"},
// //        {"http://a/b/c/d;p?q", "", "http://a/b/c/d;p?q"},
// //        {"http://a/b/c/d;p?q", ".", "http://a/b/c/"},
// //        {"http://a/b/c/d;p?q", "./", "http://a/b/c/"},
// //        {"http://a/b/c/d;p?q", "..", "http://a/b/"},
// //        {"http://a/b/c/d;p?q", "../", "http://a/b/"},
// //        {"http://a/b/c/d;p?q", "../g", "http://a/b/g"},
// //        {"http://a/b/c/d;p?q", "../..", "http://a"},
// //        {"http://a/b/c/d;p?q", "../../", "http://a"},
// //        {"http://a/b/c/d;p?q", "../../g", "http://a/g"},
// //
// //        // Here are some examples of our own.
// //        {"http://example.com", "foo", "http://example.com/foo"},
// //        {"http://example.com/", "foo", "http://example.com/foo"},
// //        {"http://example.com", "foo/", "http://example.com/foo/"},
// //        {"http://example.com/", "foo/", "http://example.com/foo/"},
// //        {"http://example.com", "/foo", "http://example.com/foo"},
// //        {"http://example.com/", "/foo", "http://example.com/foo"},
// //        {"http://example.com", "/foo/", "http://example.com/foo/"},
// //        {"http://example.com/", "/foo/", "http://example.com/foo/"},
// //        {"http://example.com/", "?foo", "http://example.com/?foo"},
// //        {"http://example.com/", "#foo", "http://example.com/#foo"},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri baseUri, relativeReferenceUri, expectedTargetUri;
// //        ASSERT_TRUE(baseUri.parseFromString(testVector.baseString));
// //        ASSERT_TRUE(relativeReferenceUri.parseFromString(testVector.relativeReferenceString)) << index;
// //        ASSERT_TRUE(expectedTargetUri.parseFromString(testVector.targetString)) << index;
// //        //const auto actualTargetUri = baseUri.Resolve(relativeReferenceUri);
// //        //ASSERT_EQ(expectedTargetUri, actualTargetUri) << index;
// //        ++index;
// //    }
// //}
// //
// //TEST(UriTests, EmptyPathInUriWithAuthorityIsEquivalentToSlashOnlyPath) {
// //    Uri::Uri uri1, uri2;
// //    ASSERT_TRUE(uri1.parseFromString("http://example.com"));
// //    ASSERT_TRUE(uri2.parseFromString("http://example.com/"));
// //    ASSERT_EQ(uri1, uri2);
// //    ASSERT_TRUE(uri1.parseFromString("//example.com"));
// //    ASSERT_TRUE(uri2.parseFromString("//example.com/"));
// //    ASSERT_EQ(uri1, uri2);
// //}
// //
// //TEST(UriTests, IPv6Address) {
// //    struct TestVector {
// //        std::string uriString;
// //        std::string expectedHost;
// //        bool isValid;
// //    };
// //    const std::vector< TestVector > testVectors{
// //        // valid
// //        {"http://[::1]/", "::1", true},
// //        {"http://[::ffff:1.2.3.4]/", "::ffff:1.2.3.4", true},
// //        {"http://[2001:db8:85a3:8d3:1319:8a2e:370:7348]/", "2001:db8:85a3:8d3:1319:8a2e:370:7348", true},
// //        {"http://[fFfF::1]", "fFfF::1", true},
// //        {"http://[fFfF:1:2:3:4:5:6:a]", "fFfF:1:2:3:4:5:6:a", true},
// //
// //        // invalid
// //        {"http://[::fFfF::1]", "", false},
// //        {"http://[::ffff:1.2.x.4]/", "", false},
// //        {"http://[::ffff:1.2.3.4.8]/", "", false},
// //        {"http://[::ffff:1.2.3]/", "", false},
// //        {"http://[::ffff:1.2.3.]/", "", false},
// //        {"http://[::ffff:1.2.3.256]/", "", false},
// //        {"http://[::fxff:1.2.3.4]/", "", false},
// //        {"http://[::ffff:1.2.3.-4]/", "", false},
// //        {"http://[::ffff:1.2.3. 4]/", "", false},
// //        {"http://[::ffff:1.2.3.4 ]/", "", false},
// //        {"http://[::ffff:1.2.3.4/", "", false},
// //        {"http://::ffff:1.2.3.4]/", "", false},
// //        {"http://::ffff:a.2.3.4]/", "", false},
// //        {"http://::ffff:1.a.3.4]/", "", false},
// //        {"http://[2001:db8:85a3:8d3:1319:8a2e:370:7348:0000]/", "", false},
// //        {"http://[2001:db8:85a3::8a2e:0:]/", "", false},
// //        {"http://[2001:db8:85a3::8a2e::]/", "", false},
// //        {"http://[]/", "", false},
// //        {"http://[:]/", "", false},
// //        {"http://[v]/", "", false},
// //    };
// //    size_t index = 0;
// //    for (const auto& testVector : testVectors) {
// //        Uri::Uri uri;
// //        const bool parseResult = uri.parseFromString(testVector.uriString);
// //        ASSERT_EQ(testVector.isValid, parseResult) << index;
// //        if (parseResult) {
// //            ASSERT_EQ(testVector.expectedHost, uri.getHost());
// //        }
// //        ++index;
// //    }
// //}
// //
//TEST(UriTests, generateString) {
//    struct TestVector {
//        std::string scheme;
//        std::string userinfo;
//        std::string host;
//        bool hasPort;
//        uint16_t port;
//        std::vector< std::string > path;
//        bool hasQuery;
//        std::string query;
//        bool hasFragment;
//        std::string fragment;
//        std::string expectedUriString;
//    };
//    const std::vector< TestVector > testVectors{
//        // general test vectors
//        {"http", "bob", "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "ch2", "http://bob@www.example.com:8080/abc/def?foobar#ch2"},
//        {"http", "bob", "www.example.com", true,  0,    {},                 true,  "foobar", true,  "ch2", "http://bob@www.example.com:0?foobar#ch2"},
//        {"http", "bob", "www.example.com", true,  0,    {},                 true,  "foobar", true,  "",    "http://bob@www.example.com:0?foobar#"},
//        {"",     "",    "example.com",     false, 0,    {},                 true,  "bar",    false, "",    "//example.com?bar"},
//        {"",     "",    "example.com",     false, 0,    {},                 true,  ""   ,    false, "",    "//example.com?"},
//        {"",     "",    "example.com",     false, 0,    {},                 false, "",       false, "",    "//example.com"},
//        {"",     "",    "example.com",     false, 0,    {""},               false, "",       false, "",    "//example.com/"},
//        {"",     "",    "example.com",     false, 0,    {"", "xyz"},        false, "",       false, "",    "//example.com/xyz"},
//        {"",     "",    "example.com",     false, 0,    {"", "xyz", ""},    false, "",       false, "",    "//example.com/xyz/"},
//        {"",     "",    "",                false, 0,    {""},               false, "",       false, "",    "/"},
//        {"",     "",    "",                false, 0,    {"", "xyz"},        false, "",       false, "",    "/xyz"},
//        {"",     "",    "",                false, 0,    {"", "xyz", ""},    false, "",       false, "",    "/xyz/"},
//        {"",     "",    "",                false, 0,    {},                 false, "",       false, "",    ""},
//        {"",     "",    "",                false, 0,    {"xyz"},            false, "",       false, "",    "xyz"},
//        {"",     "",    "",                false, 0,    {"xyz", ""},        false, "",       false, "",    "xyz/"},
//        {"",     "",    "",                false, 0,    {},                 true,  "bar",    false, "",    "?bar"},
//        {"http", "",    "",                false, 0,    {},                 true,  "bar",    false, "",    "http:?bar"},
//        {"http", "",    "",                false, 0,    {},                 false, "",       false, "",    "http:"},
//        {"http", "",    "::1",             false, 0,    {},                 false, "",       false, "",    "http://[::1]"},
//        {"http", "",    "::1.2.3.4",       false, 0,    {},                 false, "",       false, "",    "http://[::1.2.3.4]"},
//        {"http", "",    "1.2.3.4",         false, 0,    {},                 false, "",       false, "",    "http://1.2.3.4"},
//        {"",     "",    "",                false, 0,    {},                 false, "",       false, "",    ""},
//        {"http", "bob", "",                false, 0,    {},                 true,  "foobar", false, "",    "http://bob@?foobar"},
//        {"",     "bob", "",                false, 0,    {},                 true,  "foobar", false, "",    "//bob@?foobar"},
//        {"",     "bob", "",                false, 0,    {},                 false, "",       false, "",    "//bob@"},
//
//        // percent-encoded character test vectors
//        {"http", "b b", "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "ch2", "http://b%20b@www.example.com:8080/abc/def?foobar#ch2"},
//        {"http", "bob", "www.e ample.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "ch2", "http://bob@www.e%20ample.com:8080/abc/def?foobar#ch2"},
//        {"http", "bob", "www.example.com", true,  8080, {"", "a c", "def"}, true,  "foobar", true,  "ch2", "http://bob@www.example.com:8080/a%20c/def?foobar#ch2"},
//        {"http", "bob", "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foo ar", true,  "ch2", "http://bob@www.example.com:8080/abc/def?foo%20ar#ch2"},
//        {"http", "bob", "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "c 2", "http://bob@www.example.com:8080/abc/def?foobar#c%202"},
//        {"http", "bob", "ሴ.example.com",   true,  8080, {"", "abc", "def"}, true,  "foobar", false, "",    "http://bob@%E1%88%B4.example.com:8080/abc/def?foobar"},
//
//        // normalization of IPv6 address hex digits
//        {"http", "bob", "fFfF::1", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "c 2", "http://bob@[ffff::1]:8080/abc/def?foobar#c%202"},
//    };
//    size_t index = 0;
//    for (const auto& testVector : testVectors) {
//        Uri::Uri uri;
//        uri.setScheme(testVector.scheme);
//        uri.setUserInfo(testVector.userinfo);
//        uri.setHost(testVector.host);
//        if (testVector.hasPort) {
//            uri.setPort(testVector.port);
//        }
//        else {
//            uri.clearPort();
//        }
//        uri.setPath(testVector.path);
//        if (testVector.hasQuery) {
//            uri.setQuery(testVector.query);
//        }
//        else {
//            uri.clearQuery();
//        }
//        if (testVector.hasFragment) {
//            uri.setFragment(testVector.fragment);
//        }
//        else {
//            uri.clearFragment();
//        }
//        const auto actualUriString = uri.generateString();
//        ASSERT_EQ(testVector.expectedUriString, actualUriString) << index;
//        ++index;
//    }
//}
// //
// //TEST(UriTests, FragmentEmptyButPresent) {
// //    Uri::Uri uri;
// //    ASSERT_TRUE(uri.parseFromString("http://example.com#"));
// //    ASSERT_TRUE(uri.hasFragment());
// //    ASSERT_EQ("", uri.getFragment());
// //    ASSERT_EQ("http://example.com/#", uri.generateString());
// //    uri.clearFragment();
// //    ASSERT_EQ("http://example.com/", uri.generateString());
// //    ASSERT_FALSE(uri.hasFragment());
// //    ASSERT_TRUE(uri.parseFromString("http://example.com"));
// //    ASSERT_FALSE(uri.hasFragment());
// //    //uri.setFragment("");
// //    ASSERT_TRUE(uri.hasFragment());
// //    ASSERT_EQ("", uri.getFragment());
// //    ASSERT_EQ("http://example.com/#", uri.generateString());
// //}
// //
// //TEST(UriTests, QueryEmptyButPresent) {
// //    Uri::Uri uri;
// //    ASSERT_TRUE(uri.parseFromString("http://example.com?"));
// //    ASSERT_TRUE(uri.hasQuery());
// //    ASSERT_EQ("", uri.getQuery());
// //    ASSERT_EQ("http://example.com/?", uri.generateString());
// //    uri.clearQuery();
// //    ASSERT_EQ("http://example.com/", uri.generateString());
// //    ASSERT_FALSE(uri.hasQuery());
// //    ASSERT_TRUE(uri.parseFromString("http://example.com"));
// //    ASSERT_FALSE(uri.hasQuery());
// //    uri.setQuery("");
// //    ASSERT_TRUE(uri.hasQuery());
// //    ASSERT_EQ("", uri.getQuery());
// //    ASSERT_EQ("http://example.com/?", uri.generateString());
// //}
// //
// //TEST(UriTests, MakeACopy) {
// //    Uri::Uri uri1;
// //    (void)uri1.parseFromString("http://www.example.com/foo.txt");
// //    Uri::Uri uri2(uri1);
// //    uri1.setQuery("bar");
// //    /*uri2.setFragment("page2");
// //    uri2.setHost("example.com");*/
// //    EXPECT_EQ("http://www.example.com/foo.txt?bar", uri1.generateString());
// //    EXPECT_EQ("http://example.com/foo.txt#page2", uri2.generateString());
// //}
// //
// ////TEST(UriTests, AssignACopy) {
// ////    Uri::Uri uri1;
// ////    (void)uri1.parseFromString("http://www.example.com/foo.txt");
// ////    Uri::Uri uri2;
// ////    uri2 = uri1;
// ////    uri1.setQuery("bar");
// ////    uri2.setFragment("page2");
// ////    uri2.setHost("example.com");
// ////    EXPECT_EQ("http://www.example.com/foo.txt?bar", uri1.generateString());
// ////    EXPECT_EQ("http://example.com/foo.txt#page2", uri2.generateString());
// ////}
// //
// //TEST(UriTests, clearQuery) {
// //    Uri::Uri uri;
// //    (void)uri.parseFromString("http://www.example.com/?foo=bar");
// //    uri.clearQuery();
// //    EXPECT_EQ("http://www.example.com/", uri.generateString());
// //    EXPECT_FALSE(uri.hasQuery());
// //}
// //
// //TEST(UriTests, PercentEncodePlusInQueries) {
// //    // Although RFC 3986 doesn't say anything about '+', some web services
// //    // treat it the same as ' ' due to how HTML originally defined how
// //    // to encode the query portion of a URL
// //    // (see https://stackoverflow.com/questions/2678551/when-to-encode-space-to-plus-or-20).
// //    //
// //    // To avoid issues with these web services, make sure '+' is
// //    // percent-encoded in a URI when the URI is encoded.
// //    Uri::Uri uri;
// //    uri.setQuery("foo+bar");
// //    EXPECT_EQ("?foo%2Bbar", uri.generateString());
// //}