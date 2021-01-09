#include "Character_Set.hpp"

#include <gtest/gtest.h>

TEST(CharacterSetTests, ConstructorOneArgument)
{
	Character_Set charset('a');
	ASSERT_TRUE(charset.is_contains('a'));

	Character_Set charset1('$');
	ASSERT_TRUE(charset1.is_contains('$'));

	Character_Set charset2('*');
	ASSERT_TRUE(charset2.is_contains('*'));
}

TEST(CharacterSetTests, ConstructorTwoArgument)
{
	Character_Set charset('a', 'z');
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
	Character_Set charList
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


