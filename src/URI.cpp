#include "Percent_Encoding.hpp"
#include "Character_Set.hpp"
#include "URI.hpp"

#include <algorithm>
#include <functional>

namespace
{
	URI::Character_Set characterset;
}

namespace URI
{
	struct URI::Impl
	{
		// Properties
		std::string scheme;

		std::string authority;

		std::string user_info;

		std::string host;

		bool has_port_bool;
		uint16_t port;

		bool is_relative_path;
		std::vector < std::string > path;

		bool has_query_bool;
		std::string query;

		bool has_fragment_bool;
		std::string fragment;

		// Methods 
		bool parse_scheme(const std::string& uri_string, std::string & rest_string)
		{
			// Extract the scheme part(if any) from uri_string
			auto authority_or_path_start_delimiter = uri_string.find('/');
			if (authority_or_path_start_delimiter == std::string::npos)
			{
				authority_or_path_start_delimiter = uri_string.size();
			}

			auto scheme_part = uri_string.substr(0, authority_or_path_start_delimiter);
			// Find the position of ':' (if any)
			if (scheme_part.find(':') != std::string::npos)
			{
				// If we find the scheme, do
				scheme = scheme_part.substr(0, scheme_part.find(':'));

				// strip scheme part from uri_string,
				// and store it to "rest" parameter
				rest_string = uri_string.substr(scheme_part.find(':') + 1);
				return true;
			}
			else
			{
				rest_string = uri_string;
				scheme.clear();
				return true;
			}
		}
		
		bool parse_user_info(std::string& authority_string, std::string& authority_without_user_info)
		{
			auto user_info_end_delimiter = authority_string.find('@');
			if (user_info_end_delimiter != std::string::npos)
			{
				user_info = authority_string.substr(0, user_info_end_delimiter);
				authority_without_user_info = authority_string.substr(user_info_end_delimiter);
				return true;
			}

			authority_without_user_info = authority_string;
			return true;
		}

		bool parse_host(std::string& authority_without_user_info, std::string& authority_without_host)
		{
			auto host_end_delimiter = authority_without_user_info.find_first_of(":/");
			if (host_end_delimiter != std::string::npos)
			{
				// we find host part
				host = authority_without_user_info.substr(0, host_end_delimiter);
				
				authority_without_host = authority_without_user_info.substr(host_end_delimiter);
				
				return true;
			}
			else
			{
				host = authority_without_user_info;

				authority_without_host = authority_without_user_info;

				return true;
			}
		}

		bool parse_port(std::string& authority_string)
		{
			auto port_begin_delimiter = authority_string.find(':');
			if (port_begin_delimiter != std::string::npos)
			{
				// has port
				has_port_bool = true;
				// strip ':'
				std::string port_string = authority_string.substr(port_begin_delimiter+1);
				
				// whether the given port string is integer string.
				for(auto iterator = port_string.cbegin(); iterator != port_string.cend(); ++iterator)
				{
					if(!std::isdigit(*iterator))
					{
						has_port_bool = false;
						return false;
					}
				}

				int temperary_port = std::stol(port_string);

				if ( temperary_port < 0 || temperary_port >= 65536)
				{
					has_port_bool = false;
					return false;
				}
				else
				{
					port = temperary_port;
					return true;
				}
			}
			else
			{
				has_port_bool = false;
				return true;
			}
		}

		bool parse_authority(std::string& uri_string, std::string& rest_string)
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
				// If authority exists and the query or fragment exist
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

		bool parse_path(std::string& uri_may_contain_path, std::string& rest_string)
		{
			path.clear();
			if(uri_may_contain_path == "/")
			{
				// usi path begins with "/" is absolute path
				is_relative_path = false;
				path.push_back("");
				uri_may_contain_path.clear();
				rest_string = "";
				return true;
			}
			else if(!uri_may_contain_path.empty())
			{
				// The path is terminated by the first question "?", "#", or
				// by the end of the URI.
				auto path_end_delimiter = uri_may_contain_path.find_first_of("?#");

				// path string without query or fragment
				if (path_end_delimiter == std::string::npos)
				{
					// strip beginning slash of path string
					auto begin_slash_position = uri_may_contain_path.find_first_of('/');
					if(begin_slash_position == 0)
					{
						is_relative_path = false;
						uri_may_contain_path = uri_may_contain_path.substr(1);
					}
					else
					{
						is_relative_path = true;
					}
					

					for(;;)
					{
						auto path_elelment_delimiter = uri_may_contain_path.find("/");
						if(path_elelment_delimiter != std::string::npos)
						{
							path.emplace_back(uri_may_contain_path.begin(), uri_may_contain_path.begin() + path_elelment_delimiter);
							uri_may_contain_path = uri_may_contain_path.substr(path_elelment_delimiter+1);
						}
						else	// no "/" found
						{
							path.push_back(uri_may_contain_path);
							uri_may_contain_path.clear();
							break;
						}
					}
					rest_string = "";
					return true;
					
				}
				else	// find query or fragment
				{
					rest_string = uri_may_contain_path.substr(path_end_delimiter);
					
					return true;
				}
				
			}

			// else, uri is empty
			path.push_back("");

			// This is a special case,
			// If host presents and path is empty,
			// This path is absolute path
			// e.g. "https://google.com" is absolute path.
			// It's same as "https://google.com/"
			if(!host.empty() && uri_may_contain_path.empty())
			{
				is_relative_path = false;
				path.push_back("");
				return true;
			}
			
			// If host doesn't exist and path is empty,
			// The path is relative path
			is_relative_path = true;
			rest_string = uri_may_contain_path;
			return true;
		}

		bool parse_query(std::string& uri_string, std::string& rest_string)
		{
			auto query_begin_delimiter = uri_string.find('?');
			auto query_end_delimiter = uri_string.find('#');
			if (query_begin_delimiter != std::string::npos)
			{
				// has query
				has_query_bool = true;
				if (query_end_delimiter != std::string::npos)
				{
					query = uri_string.substr(query_begin_delimiter+1, query_end_delimiter-1);
					rest_string = uri_string.substr(query_end_delimiter);
					return true;
				}
				else
				{
					query = uri_string.substr(query_begin_delimiter+1);
					rest_string = "";
					return true;
				}
				
			}
			else
			{
				has_query_bool = false;
				rest_string = uri_string;
				return true;
			}
		}
		
		bool parse_fragment(std::string& uri_may_contain_fragment, std::string& rest_string)
		{
			auto fragment_begin_delimiter = uri_may_contain_fragment.find('#');
			if (fragment_begin_delimiter != std::string::npos)
			{
				// has fragment
				has_fragment_bool = true;
				fragment = uri_may_contain_fragment.substr(fragment_begin_delimiter + 1);
				rest_string = "";
				return true;
			}
			else
			{
				has_fragment_bool = false;
				rest_string = uri_may_contain_fragment;
				return true;
			}
			
		}
		
		// absolute path begins with a "/"
		bool is_absolute_path()
		{
			return (!path.empty() && !is_relative_path);
		}

		void remove_dots()
		{
			std::vector< std::string > old_path = std::move(path);
			path.clear();
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
					if(!path.empty())
					{
						// remove last path element if we can navigate up a path level 
						if(!is_absolute_path() || (path.size() > 1))
						{
							path.pop_back();
						}
					}
					is_at_directory_level = true;
				}
				else
				{
					if( !is_at_directory_level || ! segment.empty())
					{
						path.push_back(segment);
					}
					is_at_directory_level = segment.empty();
				}
				
				
			}
			
			// if at the end of reconstructing the path,
			// we are in a directory level, 
			// add an empty segment to path vector end to notify
			if( is_at_directory_level && (!path.empty() && !path.back().empty()))
			{
				path.push_back("");
			}
		}

	};
	
	URI::~URI() noexcept = default;
    
	URI::URI(const URI& other) : impl_(new Impl)
    {
        *this = other;
    }
    
	URI& URI::operator=(const URI& other) 
	{
        if(this != &other) 
		{
            *impl_ = *other.impl_;
        }
        return *this;
    }
    
	URI::URI(URI&&) noexcept = default;

	URI& URI::operator=(URI&&) noexcept = default;

    URI::URI() : impl_(new Impl)
    {
    }
	
	// Getters
	std::string URI::get_scheme()
	{
		return impl_->scheme;
	}

	std::string URI::get_user_info()
	{
		return impl_->user_info;
	}
	
	std::string URI::get_host()
	{
		return impl_->host;
	}

	std::vector< std::string > URI::get_path()
	{
		return impl_->path;
	}

	std::string URI::get_path_string()
	{
		if(impl_->path[0] == "")
			return "/";
		std::string pathString;
		for(auto &element : impl_->path)
		{
			pathString += element;
		}
		return pathString;
	}

	int URI::get_port()
	{
		return impl_->port;
	}

	std::string URI::get_query()
	{
		return impl_->query;
	}

	std::string URI::get_fragment()
	{
		return impl_->fragment;
	}

	// Setters
	bool URI::set_scheme(std::string& s)
	{
		if (impl_->scheme.empty())
		{
			impl_->scheme = s;
			return true;
		}
		return false;
	}

	bool URI::set_host(std::string& host)
	{
		if (impl_->host.empty()) 
		{ 
			impl_->host = host; 
			return true;
		}
		return false;
	}

	bool URI::set_port(int& p)
	{
		impl_->port = p;
		return true;
	}


	bool URI::set_query(std::string q)
	{
		impl_->query = q;
		if (impl_->query.empty()) { return false; }
		return true;
	}

	bool URI::set_fragment(std::string& s)
	{
		if (impl_->fragment.empty())
		{
			impl_->fragment = s;
			return true;
		}
		return false;
	}

	bool URI::clear_port()
	{
		impl_->port = -1;
		return true;
	}

	bool URI::clear_query()
	{
		if (!impl_->query.empty())
		{
			impl_->query.clear();
			return true;
		}
		return false;
	}

	bool URI::clear_fragment()
	{
		if (!impl_->fragment.empty())
		{
			impl_->fragment.clear();
			return true;
		}
		return false;
	}

	bool URI::clear_scheme()
	{
		impl_->scheme.clear();
		return true;
	}

	bool URI::has_port()
	{
		return impl_->has_port_bool;
	}

	bool URI::has_query()
	{
		return impl_->has_query_bool;
	}

	bool URI::has_fragment()
	{
		return impl_->has_fragment_bool;
	}


	bool URI::parse_from_string(const std::string& uri_string)
	{
		Percent_Encoding percent_encoding;
		std::string decoded_uri_string = percent_encoding.decode(uri_string);

		std::string uri_without_scheme;

		if(!impl_->parse_scheme(decoded_uri_string, uri_without_scheme))
		{
			return false;
		}

		std::string uri_without_authority;
		if(!impl_->parse_authority(uri_without_scheme, uri_without_authority))
		{
			return false;
		}
		
		std::string uri_without_path;
		if(!impl_->parse_path(uri_without_authority, uri_without_path))
		{
			return false;
		}
		
		std::string uri_without_query;
		if(!impl_->parse_query(uri_without_path, uri_without_query))
		{
			return false;
		}
		
		std::string uri_without_fragment;
		if(!impl_->parse_fragment(uri_without_query, uri_without_fragment))
		{
			return false;
		}

		return true;
	}

	// Bitate: the conditions of relative reference
	bool URI::is_relative_reference()
	{
		return impl_->scheme.empty();
	}

	bool URI::has_relative_path()
	{
		return !impl_->is_absolute_path();
	}

}
