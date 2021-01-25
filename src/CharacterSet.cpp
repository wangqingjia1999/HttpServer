#include "CharacterSet.hpp"

CharacterSet::~CharacterSet()
{
	m_character_set.clear();
}

CharacterSet::CharacterSet()
{
}

CharacterSet::CharacterSet(char c)
{
	m_character_set.insert(c);
}

CharacterSet::CharacterSet(std::initializer_list<const CharacterSet> character_sets)
{
	for (auto single_character_set = character_sets.begin(); single_character_set != character_sets.end(); ++single_character_set)
	{
		m_character_set.insert(single_character_set->m_character_set.begin(), single_character_set->m_character_set.end());
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