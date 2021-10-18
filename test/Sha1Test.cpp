#include "Sha1.hpp"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(sha1_tests, sha1_encode_test)
{
	struct test_vector
	{
		std::string raw_string;
		std::string message_digest;
	};

	const std::vector<test_vector> test_vectors{
	    {"abc", "a9993e364706816aba3e25717850c26c9cd0d89d"},
	    {"", "da39a3ee5e6b4b0d3255bfef95601890afd80709"},
	    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	     "84983e441c3bd26ebaae4aa1f95129e5e54670f1"},
	    {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklm"
	     "nopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
	     "a49b2446a02c645bf419f995b67091253a04a259"},
	    {std::string(1000000, 'a'), "34aa973cd4c4daa4f61eeb2bdbad27316534016f"},
	};

	size_t index = 0;
	for (auto& test_case : test_vectors)
	{
		ASSERT_EQ(Sha1::sha1_encrypt(test_case.raw_string),
		          test_case.message_digest)
		    << "Error at index: " << index;
		++index;
	}
}