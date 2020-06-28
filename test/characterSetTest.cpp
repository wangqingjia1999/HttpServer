#include <gtest/gtest.h>
#include "CharacterSet.hpp"

TEST(CharacterSetTests, ConstructorOneArgument)
{
	Uri::CharacterSet charset('a');
	ASSERT_TRUE(charset.isContains('a'));

	Uri::CharacterSet charset1('$');
	ASSERT_TRUE(charset1.isContains('$'));

	Uri::CharacterSet charset2('*');
	ASSERT_TRUE(charset2.isContains('*'));
}

TEST(CharacterSetTests, ConstructorTwoArgument)
{
	Uri::CharacterSet charset('a', 'z');
	ASSERT_TRUE(charset.isContains('b'));
	ASSERT_TRUE(charset.isContains('f'));
	ASSERT_TRUE(charset.isContains('g'));
	ASSERT_TRUE(charset.isContains('j'));
	ASSERT_TRUE(charset.isContains('w'));

	ASSERT_FALSE(charset.isContains('A'));
	ASSERT_FALSE(charset.isContains('F'));
	ASSERT_FALSE(charset.isContains('G'));
}

TEST(CharacterSetTests, ConstructorManyArguments)
{
	Uri::CharacterSet charList
	{
	'a','b','d','e','@','$','%'
	};

	ASSERT_TRUE(charList.isContains('$'));
	ASSERT_TRUE(charList.isContains('@'));
	ASSERT_TRUE(charList.isContains('%'));

	ASSERT_FALSE(charList.isContains('!'));
	ASSERT_FALSE(charList.isContains('('));
	ASSERT_FALSE(charList.isContains(']'));
}


