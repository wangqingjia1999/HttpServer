#include "Sentence.hpp"

Sentence::Sentence(const std::string& text, const std::string& publisher)
    : m_text{text},
      m_publisher{publisher},
      m_uri{}
{
}

Sentence::Sentence(const std::string& text, const std::string& uri, const std::string& publisher)
    : m_text{text},
      m_uri{uri},
      m_publisher{publisher}
{
}
