#ifndef CHARACTER_SET_HPP
#define CHARACTER_SET_HPP

#include <memory>
#include <set>
#include <initializer_list>
#include <string>

namespace URI
{
	class Character_Set
	{
		// Lifecycle management
	public:
		~Character_Set();
		Character_Set();
		
		Character_Set(const Character_Set&) = default;
		Character_Set& operator=(const Character_Set&) = default;

		Character_Set(Character_Set&&) = default;
		Character_Set& operator=(Character_Set&&) = default;

		// Methods
	public:
		/**
		 * @brief  Construct one argument.
		 * @param  c  A character.
		 */
		Character_Set(char c);

		/**
		 * @brief  Construct a list of arguments.
		 * @param  characterSets  A list of characters.
		 */
		Character_Set(std::initializer_list< const Character_Set > characterSets);

		/**
		 * @brief  Construct a range of arguments.
		 * @param  first  First character.
		 * @param  last  Last character.
		 */
		Character_Set(char first, char last);

		/**
		 * @brief  Is contain given character?
		 * @param  c  Character
		 * @return  True if contain c.
		 */
		bool is_contains(char c) const;

		/**
		 * @brief  Given string is integer string?
		 * @param  input_string  Input string.
		 * @return  True if it is integer string.
		 */
		bool isIntegerString( std::string input_string ) const;

		// Private Properties;
	public:
		std::set< char > characterSet;
	};
}

#endif