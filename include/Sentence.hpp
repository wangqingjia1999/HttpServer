#pragma once

#include <string>

class Sentence
{
public:
    Sentence() = default;
    ~Sentence() = default;

    Sentence(const Sentence& other) = default;
    Sentence& operator=(const Sentence& other) = default;

    Sentence(Sentence&& other) = default;
    Sentence& operator=(Sentence&& other) = default;

public:
    Sentence(const std::string& text, const std::string& publisher);
    Sentence(const std::string& text, const std::string& uri, const std::string& publisher);

private:
    std::string m_text;
    std::string m_uri;
    std::string m_publisher;
};