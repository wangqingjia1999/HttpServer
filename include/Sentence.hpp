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

	Sentence(const std::string& text, const std::string& publisher);
	Sentence(const std::string& text, const std::string& url,
	         const std::string& publisher);

	void set_body(const std::string& body);
	void set_url(const std::string& url);
	void set_publisher(const std::string& publisher);
	std::string get_body();
	std::string get_url();
	std::string get_publisher();

private:
	std::string m_body;
	std::string m_url;
	std::string m_publisher;
};