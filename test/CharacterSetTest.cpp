#include "CharacterSet.hpp"

#include <gtest/gtest.h>

TEST(character_set_tests, construct_one_argument)
{
	CharacterSet character_set('a');
	ASSERT_TRUE(character_set.is_contains('a'));

	CharacterSet character_set_1('$');
	ASSERT_TRUE(character_set_1.is_contains('$'));

	CharacterSet character_set_2('*');
	ASSERT_TRUE(character_set_2.is_contains('*'));

	ASSERT_TRUE(character_set != character_set_1);
	ASSERT_TRUE(character_set != character_set_2);
}

TEST(character_set_tests, construct_two_arguments)
{
	CharacterSet character_set('a', 'z');

	for (int i = static_cast<int>('a'); i <= static_cast<int>('z'); ++i)
	{
		ASSERT_TRUE(character_set.is_contains(static_cast<char>(i)));
	}

	for (int i = static_cast<int>('A'); i <= static_cast<int>('Z'); ++i)
	{
		ASSERT_FALSE(character_set.is_contains(static_cast<char>(i)));
	}
}

TEST(character_set_tests, construct_a_list_of_arguments)
{
	CharacterSet character_set_list{'a', 'b', 'd', 'e', '@', '$', '%'};

	ASSERT_TRUE(character_set_list.is_contains('$'));
	ASSERT_TRUE(character_set_list.is_contains('@'));
	ASSERT_TRUE(character_set_list.is_contains('%'));

	ASSERT_FALSE(character_set_list.is_contains('!'));
	ASSERT_FALSE(character_set_list.is_contains('('));
	ASSERT_FALSE(character_set_list.is_contains(']'));
}

TEST(character_set_tests, comparison_operators_test)
{
	CharacterSet character_set_1('A', 'D');

	CharacterSet character_set_2('a', 'd');

	ASSERT_TRUE(character_set_1 != character_set_2);

	CharacterSet character_set_3{'a', 'b', 'c', 'd'};

	ASSERT_TRUE(character_set_2 == character_set_3);
}
