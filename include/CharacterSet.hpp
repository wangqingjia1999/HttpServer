#pragma once

#include <initializer_list>
#include <memory>
#include <set>
#include <string>

class CharacterSet
{
public:
	~CharacterSet();
	CharacterSet();

	CharacterSet(const CharacterSet&) = default;
	CharacterSet& operator=(const CharacterSet&) = default;

	CharacterSet(CharacterSet&&) = default;
	CharacterSet& operator=(CharacterSet&&) = default;

public:
	bool operator==(const CharacterSet& other) const;
	bool operator!=(const CharacterSet& other) const;

public:
	/**
	 * Construct one character.
	 *
	 * @param[in] c
	 * 		A character.
	 */
	CharacterSet(char c);

	/**
	 * Construct a list of characters.
	 *
	 * @param[in] character_sets
	 * 		A list of characters.
	 */
	CharacterSet(std::initializer_list<const CharacterSet> character_sets);

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

public:
	std::set<char> m_character_set;
};