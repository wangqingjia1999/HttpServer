#include "Sentence.hpp"

Sentence::Sentence(const std::string& text, const std::string& publisher)
    : m_body{text}
    , m_publisher{publisher}
    , m_url{}
{
}

Sentence::Sentence(const std::string& text, const std::string& uri,
                   const std::string& publisher)
    : m_body{text}
    , m_url{uri}
    , m_publisher{publisher}
{
}

void Sentence::set_body(const std::string& body) { m_body = body; }
void Sentence::set_url(const std::string& url) { m_url = url; }
void Sentence::set_publisher(const std::string& publisher)
{
	m_publisher = publisher;
}
std::string Sentence::get_body() { return m_body; }
std::string Sentence::get_url() { return m_url; }
std::string Sentence::get_publisher() { return m_publisher; }