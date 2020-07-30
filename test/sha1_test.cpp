#include <gtest/gtest.h>
#include "sha1.hpp"
#include <vector>
#include <string>

TEST(sha1_tests, sha1_encode_test)
{
    std::string test1 = "abc";
    
    std::string test2a = "abcdbcdecdefdefgefghfghighijhi";
    std::string test2b = "jkijkljklmklmnlmnomnopnopq";
    std::string test2 = test2a + test2b;

    std::string test3 = "a";

    std::string test4a = "01234567012345670123456701234567";
    std::string test4b = "01234567012345670123456701234567";
    std::string test4 = test4a + test4b;

    std::vector< std::string > result_array = {
        "A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
        "84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
        "34 AA 97 3C D4 C4 DA A4 F6 1E EB 2B DB AD 27 31 65 34 01 6F",
        "DE A3 56 A2 CD DD 90 C7 A7 EC ED C5 EB B5 63 93 4F 46 04 52"
    };
}