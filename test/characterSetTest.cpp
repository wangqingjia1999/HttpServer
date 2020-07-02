#include <gtest/gtest.h>
#include "CharacterSet.hpp"

TEST(CharacterSetTests, ConstructorOneArgument)
{
	Uri::CharacterSet charset('a');
	ASSERT_TRUE(charset.is_contains('a'));

	Uri::CharacterSet charset1('$');
	ASSERT_TRUE(charset1.is_contains('$'));

	Uri::CharacterSet charset2('*');
	ASSERT_TRUE(charset2.is_contains('*'));
}

TEST(CharacterSetTests, ConstructorTwoArgument)
{
	Uri::CharacterSet charset('a', 'z');
	ASSERT_TRUE(charset.is_contains('b'));
	ASSERT_TRUE(charset.is_contains('f'));
	ASSERT_TRUE(charset.is_contains('g'));
	ASSERT_TRUE(charset.is_contains('j'));
	ASSERT_TRUE(charset.is_contains('w'));

	ASSERT_FALSE(charset.is_contains('A'));
	ASSERT_FALSE(charset.is_contains('F'));
	ASSERT_FALSE(charset.is_contains('G'));
}

TEST(CharacterSetTests, ConstructorManyArguments)
{
	Uri::CharacterSet charList
	{
	'a','b','d','e','@','$','%'
	};

	ASSERT_TRUE(charList.is_contains('$'));
	ASSERT_TRUE(charList.is_contains('@'));
	ASSERT_TRUE(charList.is_contains('%'));

	ASSERT_FALSE(charList.is_contains('!'));
	ASSERT_FALSE(charList.is_contains('('));
	ASSERT_FALSE(charList.is_contains(']'));
}


