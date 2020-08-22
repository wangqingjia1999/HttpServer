#include "UTF8.hpp"

#include <gtest/gtest.h>
#include <stdint.h>
#include <vector>

TEST(Utf8Tests, AsciiToUnicode) {
    const std::vector< UTF8::UnicodeCodePoint > expectedCodePoints{ 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    const auto actualCodePoints = UTF8::AsciiToUnicode("Hello");
    ASSERT_EQ(expectedCodePoints, actualCodePoints);
}

TEST(Utf8Tests, EncodeAscii) {
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > expectedEncoding{ 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    const auto actualEncoding = utf8.Encode(UTF8::AsciiToUnicode("Hello"));
    ASSERT_EQ(expectedEncoding, actualEncoding);
}

TEST(Utf8Tests, Symbols) {
    UTF8::UTF8 utf8;
    std::vector< uint8_t > expectedEncoding{ 0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x91, 0x2E };
    auto actualEncoding = utf8.Encode({0x0041, 0x2262, 0x0391, 0x002E}); // A≢Α.
    ASSERT_EQ(expectedEncoding, actualEncoding);
    expectedEncoding = { 0xE2, 0x82, 0xAC };
    actualEncoding = utf8.Encode({0x20AC}); // €
    ASSERT_EQ(expectedEncoding, actualEncoding);
}

TEST(Utf8Tests, EncodeJapanese) {
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > expectedEncoding{ 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E };
    const auto actualEncoding = utf8.Encode({0x65E5, 0x672C, 0x8A9E}); // 日本語
    ASSERT_EQ(expectedEncoding, actualEncoding);
}

TEST(Utf8Tests, StumpOfTreeEncoding) {  // chinese
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > expectedEncoding{ 0xF0, 0xA3, 0x8E, 0xB4 };
    const auto actualEncoding = utf8.Encode({0x233B4}); // 𣎴
    ASSERT_EQ(expectedEncoding, actualEncoding);
}

TEST(Utf8Tests, CodePointBeyondEndOfLastValidRange) {
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > replacementCharacterEncoding{ 0xEF, 0xBF, 0xBD };
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0x200000}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0x110000}));
}

TEST(Utf8Tests, HighAndLowSurrogateHalvesAreInvalid) {
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > replacementCharacterEncoding{ 0xEF, 0xBF, 0xBD };
    ASSERT_EQ((std::vector< uint8_t >{0xED, 0x9F, 0xBF}), utf8.Encode({0xD7FF}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xD800}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xD801}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xD803}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xDFEF}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xDFFE}));
    ASSERT_EQ(replacementCharacterEncoding, utf8.Encode({0xDFFF}));
    ASSERT_EQ((std::vector< uint8_t >{0xEE, 0x80, 0x80}), utf8.Encode({0xE000}));
}

TEST(Utf8Tests, DecodeValidSequences) {
    struct TestVector {
        std::string encoding;
        std::vector< UTF8::UnicodeCodePoint > expectedDecoding;
    };
    const std::vector< TestVector > testVectors{
        { "𣎴",     { 0x233B4 } },
        { "日本語", { 0x65E5, 0x672C, 0x8A9E } },
        { "A≢Α.",   { 0x0041, 0x2262, 0x0391, 0x002E } },
        { "€",      { 0x20AC } },
        { "Hello",  { 0x48, 0x65, 0x6C, 0x6C, 0x6F }},
    };
    for (const auto& testVector: testVectors) {
        UTF8::UTF8 utf8;
        const auto actualDecoding = utf8.Decode(testVector.encoding);
        ASSERT_EQ(testVector.expectedDecoding, actualDecoding);
    }
}

TEST(Utf8Tests, DecodeFromInputVector) {
    UTF8::UTF8 utf8;
    const auto actualDecoding = utf8.Decode(std::vector< uint8_t >{0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E});
    ASSERT_EQ((std::vector< UTF8::UnicodeCodePoint >{0x65E5, 0x672C, 0x8A9E}), actualDecoding);
}

TEST(Utf8Tests, UnexpectedContinuationBytes) {
    UTF8::UTF8 utf8;
    ASSERT_EQ(
        (std::vector< UTF8::UnicodeCodePoint >{0x0041, 0x2262, 0xFFFD, 0x002E}),
        utf8.Decode(std::vector< uint8_t >{ 0x41, 0xE2, 0x89, 0xA2, 0x91, 0x2E })
    ); // A≢�.
}

TEST(Utf8Tests, DecodeBreakInSequence) {
    UTF8::UTF8 utf8;
    ASSERT_EQ(
        (std::vector< UTF8::UnicodeCodePoint >{0x0041, 0x2262, 0xFFFD, 0x002E}),
        utf8.Decode(std::vector< uint8_t >{ 0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x2E })
    ); // A≢�.
}

TEST(Utf8Tests, RejectOverlongSequences) {
    const std::vector< std::vector< uint8_t > > testVectors{
        // All U+2F ('/') -- should only need 1 byte
        { 0xc0, 0xaf },
        { 0xe0, 0x80, 0xaf },
        { 0xf0, 0x80, 0x80, 0xaf },

        // One less than the minimum code point value
        // that should require this many encoded bytes
        { 0xc1, 0xbf }, // U+7F (should be 1 byte)
        { 0xe0, 0x9f, 0xbf }, // U+7FF (should be 2 bytes)
        { 0xf0, 0x8f, 0xbf, 0xbf }, // U+FFFF (should be 3 bytes)
    };
    size_t index = 0;
    for (const auto& testVector: testVectors) {
        UTF8::UTF8 utf8;
        ASSERT_EQ(
            (std::vector< UTF8::UnicodeCodePoint >{0xFFFD}),
            utf8.Decode(testVector)
        ) << index;
        ++index;
    }
}

TEST(Utf8Tests, StumpOfTreeDecodedInTwoParts) {
    UTF8::UTF8 utf8;
    const std::vector< uint8_t > firstHalfOfEncoding{ 0xF0, 0xA3 };
    const std::vector< uint8_t > lastHalfOfEncoding{ 0x8E, 0xB4 };
    ASSERT_EQ(
        (std::vector< UTF8::UnicodeCodePoint >{}),
        utf8.Decode(firstHalfOfEncoding)
    );
    ASSERT_EQ(
        (std::vector< UTF8::UnicodeCodePoint >{0x233B4}), // 𣎴
        utf8.Decode(lastHalfOfEncoding)
    );
}

TEST(Utf8Tests, IsValidEncoding) {
    UTF8::UTF8 utf8;
    EXPECT_TRUE(utf8.IsValidEncoding("abc"));
    EXPECT_TRUE(utf8.IsValidEncoding("𣎴"));
    EXPECT_TRUE(utf8.IsValidEncoding("A≢�"));
    EXPECT_FALSE(utf8.IsValidEncoding("\x41\xE2\x89\xA2\xCE\x2E"));
    EXPECT_FALSE(utf8.IsValidEncoding("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA"));
    EXPECT_TRUE(utf8.IsValidEncoding("\xE6\x97\xA5\xE6\x9C\xAC\xE8", false));
    EXPECT_FALSE(utf8.IsValidEncoding("\xAA"));
    EXPECT_TRUE(utf8.IsValidEncoding("A≢", false));
    EXPECT_TRUE(utf8.IsValidEncoding("�"));
}
