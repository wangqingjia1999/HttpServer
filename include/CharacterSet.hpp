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
		CharacterSet(const CharacterSet&) = default;
		CharacterSet(CharacterSet&&) = default;
		CharacterSet& operator=(const CharacterSet&) = default;
		CharacterSet& operator=(CharacterSet&&) = default;

		// Methods
	public:
		CharacterSet();

		/**
		 * Construct one argument.
		 */
		CharacterSet(char c);

		/**
		 *
		 * Construct a list of arguments.
		 */
		CharacterSet(std::initializer_list< const CharacterSet > characterSets);

		/**
		 * Construct a range of arguments.
		 */
		CharacterSet(char first, char last);

		/**
		 * Is contain given character?
		 */
		bool is_contains(char c) const;

		/**
		 * Is the given string integers
		 */
		bool isIntegerString( std::string inputString ) const;

		// Private Properties;
	public:
		std::set< char > characterSet;
	};
}