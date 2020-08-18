#include "CharacterSet.hpp"

Uri::CharacterSet::~CharacterSet()
{
}

Uri::CharacterSet::CharacterSet()
{
}

Uri::CharacterSet::CharacterSet(char c)
{
	characterSet.insert(c);
}

Uri::CharacterSet::CharacterSet(std::initializer_list<const CharacterSet> characterSets)
{
	for (auto singleCharacterSet = characterSets.begin(); singleCharacterSet != characterSets.end(); ++singleCharacterSet)
	{
		characterSet.insert(singleCharacterSet->characterSet.begin(), singleCharacterSet->characterSet.end());
	}
}

Uri::CharacterSet::CharacterSet(char first, char last)
{
	// if first char greater than last char
	// swap position
	if (first > last)
	{
		std::swap(first, last);
	}
	
	// insert each char into characterSet
	for (char c = first; c < last + 1; ++c)
	{
		characterSet.insert(c);
	}
}

bool Uri::CharacterSet::is_contains(char c) const
{
	if (characterSet.find(c) != characterSet.cend())
	{
		return true;
	}
	return false;
}