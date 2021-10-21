#include "CharacterSet.hpp"

CharacterSet::CharacterSet(char c) { m_character_set.insert(c); }

CharacterSet::CharacterSet(
    std::initializer_list<const CharacterSet> character_sets)
{
	for (const auto& character_set : character_sets)
	{
		for (const auto& character : character_set.get_character_set())
		{
			m_character_set.insert(character);
		}
	}
}

CharacterSet::CharacterSet(char first, char last)
{
	if (first > last)
	{
		std::swap(first, last);
	}

	for (char c = first; c <= last; ++c)
	{
		m_character_set.insert(c);
	}
}

CharacterSet::CharacterSet(std::initializer_list<const char> characters)
{
	for (const auto character : characters)
	{
		m_character_set.insert(character);
	}
}

bool CharacterSet::operator==(const CharacterSet& other) const
{
	return this->m_character_set == other.m_character_set;
}

bool CharacterSet::operator!=(const CharacterSet& other) const
{
	return this->m_character_set != other.m_character_set;
}

bool CharacterSet::is_contains(char c) const
{
	return m_character_set.find(c) != m_character_set.cend();
}

std::set<char> CharacterSet::get_character_set() const
{
	return m_character_set;
}