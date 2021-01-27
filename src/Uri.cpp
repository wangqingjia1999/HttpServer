#include "Uri.hpp"

Uri::Uri()
	: m_has_port(false),
	  m_is_relative_path(false),
	  m_has_query(false),
	  m_has_fragment(false)
{
}

Uri::~Uri()
{
}

bool Uri::parse_scheme(const std::string& uri_string, std::string & rest_string)
{
	// Extract the m_scheme part(if any) from uri_string
	auto authority_or_path_start_delimiter = uri_string.find('/');
	if (authority_or_path_start_delimiter == std::string::npos)
	{
		authority_or_path_start_delimiter = uri_string.size();
	}

	auto scheme_part = uri_string.substr(0, authority_or_path_start_delimiter);
	// Find the position of ':' (if any)
	if (scheme_part.find(':') != std::string::npos)
	{
		// If we find the m_scheme, do
		m_scheme = scheme_part.substr(0, scheme_part.find(':'));

		// strip m_scheme part from uri_string,
		// and store it to "rest" parameter
		rest_string = uri_string.substr(scheme_part.find(':') + 1);
		return true;
	}
	else
	{
		rest_string = uri_string;
		m_scheme.clear();
		return true;
	}
}

bool Uri::parse_user_info(std::string& authority_string, std::string& authority_without_user_info)
{
	auto user_info_end_delimiter = authority_string.find('@');
	if (user_info_end_delimiter != std::string::npos)
	{
		m_user_info = authority_string.substr(0, user_info_end_delimiter);
		authority_without_user_info = authority_string.substr(user_info_end_delimiter);
		return true;
	}

	authority_without_user_info = authority_string;
	return true;
}

bool Uri::parse_host(std::string& authority_without_user_info, std::string& authority_without_host)
{
	auto host_end_delimiter = authority_without_user_info.find_first_of(":/");
	if (host_end_delimiter != std::string::npos)
	{
		// we find m_host part
		m_host = authority_without_user_info.substr(0, host_end_delimiter);
		
		authority_without_host = authority_without_user_info.substr(host_end_delimiter);
		
		return true;
	}
	else
	{
		m_host = authority_without_user_info;

		authority_without_host = authority_without_user_info;

		return true;
	}
}

bool Uri::parse_port(std::string& authority_string)
{
	auto port_begin_delimiter = authority_string.find(':');
	if (port_begin_delimiter != std::string::npos)
	{
		// has m_port
		m_has_port = true;
		// strip ':'
		std::string port_string = authority_string.substr(port_begin_delimiter+1);
		
		// whether the given m_port string is integer string.
		for(auto iterator = port_string.cbegin(); iterator != port_string.cend(); ++iterator)
		{
			if(!std::isdigit(*iterator))
			{
				m_has_port = false;
				return false;
			}
		}

		int temperary_port = std::stol(port_string);

		if ( temperary_port < 0 || temperary_port >= 65536)
		{
			m_has_port = false;
			return false;
		}
		else
		{
			m_port = temperary_port;
			return true;
		}
	}
	else
	{
		m_has_port = false;
		return true;
	}
}

bool Uri::parse_authority(std::string& uri_string, std::string& rest_string)
{
	auto authority_begin_delimiter = uri_string.find("//");

	if (authority_begin_delimiter!=std::string::npos)
	{
		// strip two slash precede authority part
		uri_string = uri_string.substr(2);
	}

	// find authority part
	if (authority_begin_delimiter != std::string::npos)
	{
		auto authority_end_delimiter = uri_string.find_first_of("/?#");
		std::string authority_string;
		// If authority exists and the m_query or m_fragment exist
		if(authority_end_delimiter != std::string::npos)
		{
			authority_string = uri_string.substr(0, authority_end_delimiter);
			rest_string = uri_string.substr(authority_end_delimiter);
		}
		else
		{
			authority_string = uri_string;
			rest_string = "";
		}

		std::string string_without_user_info;
		if(!parse_user_info(authority_string, string_without_user_info))
		{
			return false;
		}
		
		std::string string_without_host;
		if(!parse_host(string_without_user_info, string_without_host))
		{
			return false;
		}
		
		if(!parse_port(string_without_host)) 
		{
			return false;
		}		

		return true;		
	}
	else	// not find authority part
	{
		rest_string = uri_string;
		return true;
	}
}

bool Uri::parse_path(std::string& uri_may_contain_path, std::string& rest_string)
{
	m_path.clear();
	if(uri_may_contain_path == "/")
	{
		// usi m_path begins with "/" is absolute m_path
		m_is_relative_path = false;
		m_path.push_back("");
		uri_may_contain_path.clear();
		rest_string = "";
		return true;
	}
	else if(!uri_may_contain_path.empty())
	{
		// The m_path is terminated by the first question "?", "#", or
		// by the end of the Uri.
		auto path_end_delimiter = uri_may_contain_path.find_first_of("?#");

		// m_path string without m_query or m_fragment
		if (path_end_delimiter == std::string::npos)
		{
			// strip beginning slash of m_path string
			auto begin_slash_position = uri_may_contain_path.find_first_of('/');
			if(begin_slash_position == 0)
			{
				m_is_relative_path = false;
				uri_may_contain_path = uri_may_contain_path.substr(1);
			}
			else
			{
				m_is_relative_path = true;
			}
			

			for(;;)
			{
				auto path_elelment_delimiter = uri_may_contain_path.find("/");
				if(path_elelment_delimiter != std::string::npos)
				{
					m_path.emplace_back(uri_may_contain_path.begin(), uri_may_contain_path.begin() + path_elelment_delimiter);
					uri_may_contain_path = uri_may_contain_path.substr(path_elelment_delimiter+1);
				}
				else	// no "/" found
				{
					m_path.push_back(uri_may_contain_path);
					uri_may_contain_path.clear();
					break;
				}
			}
			rest_string = "";
			return true;
			
		}
		else	// find m_query or m_fragment
		{
			rest_string = uri_may_contain_path.substr(path_end_delimiter);
			
			return true;
		}
		
	}

	// else, uri is empty
	m_path.push_back("");

	// This is a special case,
	// If m_host presents and m_path is empty,
	// This m_path is absolute m_path
	// e.g. "https://google.com" is absolute m_path.
	// It's same as "https://google.com/"
	if(!m_host.empty() && uri_may_contain_path.empty())
	{
		m_is_relative_path = false;
		m_path.push_back("");
		return true;
	}
	
	// If m_host doesn't exist and m_path is empty,
	// The m_path is relative m_path
	m_is_relative_path = true;
	rest_string = uri_may_contain_path;
	return true;
}

bool Uri::parse_query(std::string& uri_string, std::string& rest_string)
{
	auto query_begin_delimiter = uri_string.find('?');
	auto query_end_delimiter = uri_string.find('#');
	if (query_begin_delimiter != std::string::npos)
	{
		// has m_query
		m_has_query = true;
		if (query_end_delimiter != std::string::npos)
		{
			m_query = uri_string.substr(query_begin_delimiter+1, query_end_delimiter-1);
			rest_string = uri_string.substr(query_end_delimiter);
			return true;
		}
		else
		{
			m_query = uri_string.substr(query_begin_delimiter+1);
			rest_string = "";
			return true;
		}
		
	}
	else
	{
		m_has_query = false;
		rest_string = uri_string;
		return true;
	}
}

bool Uri::parse_fragment(std::string& uri_may_contain_fragment, std::string& rest_string)
{
	auto fragment_begin_delimiter = uri_may_contain_fragment.find('#');
	if (fragment_begin_delimiter != std::string::npos)
	{
		// has m_fragment
		m_has_fragment = true;
		m_fragment = uri_may_contain_fragment.substr(fragment_begin_delimiter + 1);
		rest_string = "";
		return true;
	}
	else
	{
		m_has_fragment = false;
		rest_string = uri_may_contain_fragment;
		return true;
	}
	
}

// absolute m_path begins with a "/"
bool Uri::is_absolute_path()
{
	return (!m_path.empty() && !m_is_relative_path);
}

void Uri::remove_dots()
{
	std::vector< std::string > old_path = std::move(m_path);
	m_path.clear();
	// TAT: why use this?
	// Is this means that the resources directory
	bool is_at_directory_level = false;

	for(const std::string segment : old_path)
	{
		if(segment == ".")
		{
			is_at_directory_level = true;
		}
		else if ( segment == "..")
		{
			if(!m_path.empty())
			{
				// remove last m_path element if we can navigate up a m_path level 
				if(!is_absolute_path() || (m_path.size() > 1))
				{
					m_path.pop_back();
				}
			}
			is_at_directory_level = true;
		}
		else
		{
			if( !is_at_directory_level || ! segment.empty())
			{
				m_path.push_back(segment);
			}
			is_at_directory_level = segment.empty();
		}
		
		
	}
	
	// if at the end of reconstructing the m_path,
	// we are in a directory level, 
	// add an empty segment to m_path vector end to notify
	if( is_at_directory_level && (!m_path.empty() && !m_path.back().empty()))
	{
		m_path.push_back("");
	}
}

// Getters
std::string Uri::get_scheme()
{
	return m_scheme;
}

std::string Uri::get_user_info()
{
	return m_user_info;
}

std::string Uri::get_host()
{
	return m_host;
}

std::vector< std::string > Uri::get_path()
{
	return m_path;
}

std::string Uri::get_path_string()
{
	if(m_path[0] == "")
		return "/";
	std::string pathString;
	for(auto &element : m_path)
	{
		pathString += element;
	}
	return pathString;
}

int Uri::get_port()
{
	return m_port;
}

std::string Uri::get_query()
{
	return m_query;
}

std::string Uri::get_fragment()
{
	return m_fragment;
}

// Setters
bool Uri::set_scheme(std::string& s)
{
	if (m_scheme.empty())
	{
		m_scheme = s;
		return true;
	}
	return false;
}

bool Uri::set_host(std::string& m_host)
{
	if (m_host.empty()) 
	{ 
		m_host = m_host; 
		return true;
	}
	return false;
}

bool Uri::set_port(int& p)
{
	m_port = p;
	return true;
}


bool Uri::set_query(std::string q)
{
	m_query = q;
	if (m_query.empty()) { return false; }
	return true;
}

bool Uri::set_fragment(std::string& s)
{
	if (m_fragment.empty())
	{
		m_fragment = s;
		return true;
	}
	return false;
}

bool Uri::clear_port()
{
	m_port = -1;
	return true;
}

bool Uri::clear_query()
{
	if (!m_query.empty())
	{
		m_query.clear();
		return true;
	}
	return false;
}

bool Uri::clear_fragment()
{
	if (!m_fragment.empty())
	{
		m_fragment.clear();
		return true;
	}
	return false;
}

bool Uri::clear_scheme()
{
	m_scheme.clear();
	return true;
}

bool Uri::has_port()
{
	return m_has_port;
}

bool Uri::has_query()
{
	return m_has_query;
}

bool Uri::has_fragment()
{
	return m_has_fragment;
}


bool Uri::parse_from_string(const std::string& uri_string)
{
	PercentEncoding percent_encoding;
	std::string decoded_uri_string = percent_encoding.decode(uri_string);

	std::string uri_without_scheme;

	if(!parse_scheme(decoded_uri_string, uri_without_scheme))
	{
		return false;
	}

	std::string uri_without_authority;
	if(!parse_authority(uri_without_scheme, uri_without_authority))
	{
		return false;
	}
	
	std::string uri_without_path;
	if(!parse_path(uri_without_authority, uri_without_path))
	{
		return false;
	}
	
	std::string uri_without_query;
	if(!parse_query(uri_without_path, uri_without_query))
	{
		return false;
	}
	
	std::string uri_without_fragment;
	if(!parse_fragment(uri_without_query, uri_without_fragment))
	{
		return false;
	}

	return true;
}

bool Uri::is_relative_reference()
{
	return m_scheme.empty();
}

bool Uri::has_relative_path()
{
	return !is_absolute_path();
}

bool Uri::operator==(const Uri& other) const
{
	return true;
}
