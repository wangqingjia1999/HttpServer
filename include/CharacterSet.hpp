#pragma once

#include <memory>
#include <set>
#include <initializer_list>

namespace Uri
{
	class CharacterSet
	{
		// Lifecycle management
	public:
		~CharacterSet();
		CharacterSet();
		
		CharacterSet(const CharacterSet&) = default;
		CharacterSet& operator=(const CharacterSet&) = default;

		CharacterSet(CharacterSet&&) = default;
		CharacterSet& operator=(CharacterSet&&) = default;

		// Methods
	public:
		/**
		 * @brief  Construct one argument.
		 * @param  c  A character.
		 */
		CharacterSet(char c);

		/**
		 * @brief  Construct a list of arguments.
		 * @param  characterSets  A list of characters.
		 */
		CharacterSet(std::initializer_list< const CharacterSet > characterSets);

		/**
		 * @brief  Construct a range of arguments.
		 * @param  first  First character.
		 * @param  last  Last character.
		 */
		CharacterSet(char first, char last);

		/**
		 * @brief  Is contain given character?
		 * @param  c  Character
		 * @return  True if contain c.
		 */
		bool is_contains(char c) const;

		/**
		 * @brief  Given string is integer string?
		 * @param  inputString  Input string.
		 * @return  True if it is integer string.
		 */
		bool isIntegerString( std::string inputString ) const;

		// Private Properties;
	public:
		std::set< char > characterSet;
	};
}