#pragma once

#include <initializer_list>
#include <memory>
#include <set>
#include <string>

class CharacterSet
{
public:
	~CharacterSet() = default;
	CharacterSet() = default;

	CharacterSet(const CharacterSet&) = default;
	CharacterSet& operator=(const CharacterSet&) = default;

	CharacterSet(CharacterSet&&) = default;
	CharacterSet& operator=(CharacterSet&&) = default;

	bool operator==(const CharacterSet& other) const;
	bool operator!=(const CharacterSet& other) const;

	/**
	 * Construct one character.
	 *
	 * @param[in] c
	 * 		A character.
	 */
	explicit CharacterSet(char c);

	/**
	 * Construct a list of characters.
	 *
	 * @param[in] character_sets
	 * 		A list of characters.
	 */
	CharacterSet(std::initializer_list<const CharacterSet> character_sets);

	/**
	 * @brief Construct a new Character Set object
	 *
	 * @param characters
	 * 		characters to be inserted into set
	 */
	CharacterSet(std::initializer_list<const char> characters);

	/**
	 * Construct a range of arguments.
	 *
	 * @param[in] first
	 * 		First character.
	 *
	 * @param[in] last
	 * 		Last character.
	 */
	CharacterSet(char first, char last);

	/**
	 * Is contain given character?
	 *
	 * @param[in] c
	 * 		Character
	 *
	 * @return
	 * 		True if contain c.
	 */
	bool is_contains(char c) const;

	/**
	 * @brief Get the character set object
	 *
	 * @return std::set<char>
	 */
	std::set<char> get_character_set() const;

private:
	std::set<char> m_character_set;
};