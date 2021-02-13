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

bool Uri::parse_scheme(const std::string& uri, std::string & remains)
{
	// Extract the m_scheme part(if any) from uri
	auto authority_or_path_start_delimiter = uri.find('/');
	if (authority_or_path_start_delimiter == std::string::npos)
	{
		authority_or_path_start_delimiter = uri.size();
	}

	auto scheme_part = uri.substr(0, authority_or_path_start_delimiter);
	// Find the position of ':' (if any)
	if (scheme_part.find(':') != std::string::npos)
	{
		// If we find the m_scheme, do
		m_scheme = scheme_part.substr(0, scheme_part.find(':'));

		// strip m_scheme part from uri,
		// and store it to "rest" parameter
		remains = uri.substr(scheme_part.find(':') + 1);
		return true;
	}
	else
	{
		remains = uri;
		m_scheme.clear();
		return true;
	}
}

bool Uri::parse_user_info(std::string& authority, std::string& remains)
{
	auto user_info_end_delimiter = authority.find('@');
	if (user_info_end_delimiter != std::string::npos)
	{
		m_user_info = authority.substr(0, user_info_end_delimiter);
		remains = authority.substr(user_info_end_delimiter);
		return true;
	}

	remains = authority;
	return true;
}

bool Uri::parse_host(std::string& authority, std::string& remains)
{
	auto host_end_delimiter = authority.find_first_of(":/");
	if (host_end_delimiter != std::string::npos)
	{
		// we find m_host part
		m_host = authority.substr(0, host_end_delimiter);
		
		remains = authority.substr(host_end_delimiter);
		
		return true;
	}
	else
	{
		m_host = authority;

		remains = authority;

		return true;
	}
}

bool Uri::parse_port(std::string& authority)
{
	auto port_begin_delimiter = authority.find(':');
	if (port_begin_delimiter != std::string::npos)
	{
		// has m_port
		m_has_port = true;
		// strip ':'
		std::string port_string = authority.substr(port_begin_delimiter+1);
		
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

bool Uri::parse_authority(std::string& uri, std::string& remains)
{
	auto authority_begin_delimiter = uri.find("//");

	if (authority_begin_delimiter!=std::string::npos)
	{
		// strip two slash precede authority part
		uri = uri.substr(2);
	}

	// find authority part
	if (authority_begin_delimiter != std::string::npos)
	{
		auto authority_end_delimiter = uri.find_first_of("/?#");
		std::string authority;
		// If authority exists and the m_query or m_fragment exist
		if(authority_end_delimiter != std::string::npos)
		{
			authority = uri.substr(0, authority_end_delimiter);
			remains = uri.substr(authority_end_delimiter);
		}
		else
		{
			authority = uri;
			remains = "";
		}

		std::string string_without_user_info;
		if(!parse_user_info(authority, string_without_user_info))
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
		remains = uri;
		return true;
	}
}

bool Uri::parse_path(std::string& uri, std::string& remains)
{
	m_path.clear();

	if(uri == "/")
	{
		m_is_relative_path = false;
		m_path.push_back("");
		uri.clear();
		remains = "";
		return true;
	}
	else if(!uri.empty())
	{
		// The m_path is terminated by the first question "?", "#", or
		// by the end of the Uri.
		auto path_end_delimiter = uri.find_first_of("?#");

		// m_path string without m_query or m_fragment
		if (path_end_delimiter == std::string::npos)
		{
			// strip beginning slash of m_path string
			auto begin_slash_position = uri.find_first_of('/');
			if(begin_slash_position == 0)
			{
				m_is_relative_path = false;
				uri = uri.substr(1);
			}
			else
			{
				m_is_relative_path = true;
			}
			
			for(;;)
			{
				auto path_elelment_delimiter = uri.find("/");
				if(path_elelment_delimiter != std::string::npos)
				{
					m_path.emplace_back(uri.begin(), uri.begin() + path_elelment_delimiter);
					uri = uri.substr(path_elelment_delimiter+1);
				}
				else	// no "/" found
				{
					m_path.push_back(uri);
					uri.clear();
					break;
				}
			}

			remains = "";
			return true;
		}
		else	// find m_query or m_fragment
		{
			remains = uri.substr(path_end_delimiter);
			
			return true;
		}
		
	}

	// else, uri is empty
	m_path.push_back("");

	// This is a special case,
	// If m_host presents and m_path is empty,
	// This m_path is absolute m_path
	// e.g. "https://google.com" is absolute m_path.
	// It'fragment same as "https://google.com/"
	if(!m_host.empty() && uri.empty())
	{
		m_is_relative_path = false;
		m_path.push_back("");
		return true;
	}
	
	// If m_host doesn't exist and m_path is empty,
	// The m_path is relative m_path
	m_is_relative_path = true;
	remains = uri;
	return true;
}

bool Uri::parse_query(std::string& uri, std::string& remains)
{
	auto query_begin_delimiter = uri.find('?');
	auto query_end_delimiter = uri.find('#');
	if (query_begin_delimiter != std::string::npos)
	{
		m_has_query = true;
		if (query_end_delimiter != std::string::npos)
		{
			m_query = uri.substr(query_begin_delimiter+1, query_end_delimiter-1);
			remains = uri.substr(query_end_delimiter);
			return true;
		}
		else
		{
			m_query = uri.substr(query_begin_delimiter+1);
			remains = "";
			return true;
		}
	}
	else
	{
		m_has_query = false;
		remains = uri;
		return true;
	}
}

bool Uri::parse_fragment(std::string& uri, std::string& remains)
{
	auto fragment_begin_delimiter = uri.find('#');
	if (fragment_begin_delimiter != std::string::npos)
	{
		// has m_fragment
		m_has_fragment = true;
		m_fragment = uri.substr(fragment_begin_delimiter + 1);
		remains = "";
		return true;
	}
	else
	{
		m_has_fragment = false;
		remains = uri;
		return true;
	}
	
}

// absolute m_path begins with a "/"
bool Uri::is_absolute_path()
{
	return (!m_path.empty() && !m_is_relative_path);
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
bool Uri::set_scheme(std::string& scheme)
{
	if (m_scheme.empty())
	{
		m_scheme = scheme;
		return true;
	}
	return false;
}

bool Uri::set_host(std::string& host)
{
	if (m_host.empty()) 
	{ 
		m_host = host; 
		return true;
	}
	return false;
}

bool Uri::set_port(int port)
{
	m_port = port;
	return true;
}


bool Uri::set_query(std::string query)
{
	m_query = query;
	if (m_query.empty()) { return false; }
	return true;
}

bool Uri::set_fragment(std::string& fragment)
{
	if (m_fragment.empty())
	{
		m_fragment = fragment;
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


bool Uri::parse_from_string(const std::string& uri)
{
	PercentEncoding percent_encoding;
	std::string decoded_uri_string = percent_encoding.decode(uri);

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
