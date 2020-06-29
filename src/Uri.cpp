#include "PercentEncoding.hpp"
#include "CharacterSet.hpp"
#include "Uri.hpp"
#include <algorithm>
#include <functional>

namespace
{
	Uri::CharacterSet characterset;
}

namespace Uri
{
	struct Uri::Impl
	{
		// Properties
		std::string scheme;

		std::string authority;

		std::string userInfo;

		std::string host;

		bool hasPortBool;
		uint16_t port;

		bool isRelativePath;
		std::vector < std::string > path;

		bool hasQueryBool;
		std::string query;

		bool hasFragmentBool;
		std::string fragment;

		// Methods 
		bool parseScheme(const std::string& uriString, std::string & restString)
		{
			// Extract the scheme part(if any) from uriString
			auto authorityOrPathStartDelimiter = uriString.find('/');
			if (authorityOrPathStartDelimiter == std::string::npos)
			{
				authorityOrPathStartDelimiter = uriString.size();
			}

			auto schemePart = uriString.substr(0, authorityOrPathStartDelimiter);
			// Find the position of ':' (if any)
			if (schemePart.find(':') != std::string::npos)
			{
				// If we find the scheme, do
				scheme = schemePart.substr(0, schemePart.find(':'));

				// strip scheme part from uriString,
				// and store it to "rest" parameter
				restString = uriString.substr(schemePart.find(':') + 1);
				return true;
			}
			else
			{
				restString = uriString;
				scheme.clear();
				return true;
			}
		}
		
		bool parseUserInfo(std::string& authorityString, std::string& authorityStringWithoutUserInfo)
		{
			auto userInfoEndDelimiter = authorityString.find('@');
			if (userInfoEndDelimiter != std::string::npos)
			{
				userInfo = authorityString.substr(0, userInfoEndDelimiter);
				authorityStringWithoutUserInfo = authorityString.substr(userInfoEndDelimiter);
				return true;
			}

			authorityStringWithoutUserInfo = authorityString;
			return true;
		}

		bool parseHost(std::string& authorityStringWithoudUserInfo, std::string& authorityStringWithoudHost)
		{
			auto hostEndDelimiter = authorityStringWithoudUserInfo.find_first_of(":/");
			if (hostEndDelimiter != std::string::npos)
			{
				// we find host part
				host = authorityStringWithoudUserInfo.substr(0, hostEndDelimiter);
				
				authorityStringWithoudHost = authorityStringWithoudUserInfo.substr(hostEndDelimiter);
				
				return true;
			}
			else
			{
				host = authorityStringWithoudUserInfo;

				authorityStringWithoudHost = authorityStringWithoudUserInfo;

				return true;
			}
		}

		bool parsePort(std::string& authorityString)
		{
			auto portBeginDelimiter = authorityString.find(':');
			if (portBeginDelimiter != std::string::npos)
			{
				// has port
				hasPortBool = true;
				// strip ':'
				std::string portString = authorityString.substr(portBeginDelimiter+1);
				
				// whether the given port string is integer string.
				for(auto iterator = portString.cbegin(); iterator != portString.cend(); ++iterator)
				{
					if(!std::isdigit(*iterator))
					{
						hasPortBool = false;
						return false;
					}
				}

				int tempPort = std::stol(portString);

				if ( tempPort < 0 || tempPort >= 65536)
				{
					hasPortBool = false;
					return false;
				}
				else
				{
					port = tempPort;
					return true;
				}
			}
			else
			{
				hasPortBool = false;
				return true;
			}
		}

		bool parseAuthority(std::string& uriString, std::string& restString)
		{
			auto authorityBeginDelimiter = uriString.find("//");

			if (authorityBeginDelimiter!=std::string::npos)
			{
				// strip two slash precede authority part
				uriString = uriString.substr(2);
			}

			// find authority part
			if (authorityBeginDelimiter != std::string::npos)
			{
				auto authorityEndDelimiter = uriString.find_first_of("/?#");
				std::string authorityString;
				// If authority exists and the query or fragment exist
				if(authorityEndDelimiter != std::string::npos)
				{
					authorityString = uriString.substr(0, authorityEndDelimiter);
					restString = uriString.substr(authorityEndDelimiter);
				}
				else
				{
					authorityString = uriString;
					restString = "";
				}

				std::string stringWithoutUserInfo;
				if(!parseUserInfo(authorityString, stringWithoutUserInfo))
				{
					return false;
				}
				
				std::string stringWithoutHost;
				if(!parseHost(stringWithoutUserInfo, stringWithoutHost))
				{
					return false;
				}
				
				if(!parsePort(stringWithoutHost)) 
				{
					return false;
				}		

				return true;		
			}
			else	// not find authority part
			{
				restString = uriString;
				return true;
			}
		}

		bool parsePath(std::string& uriMayCantainPath, std::string& restString)
		{
			path.clear();
			if(uriMayCantainPath == "/")
			{
				// usi path begins with "/" is absolute path
				isRelativePath = false;
				path.push_back("");
				uriMayCantainPath.clear();
				restString = "";
				return true;
			}
			else if(!uriMayCantainPath.empty())
			{
				// The path is terminated by the first question "?", "#", or
				// by the end of the URI.
				auto pathEndDelimiter = uriMayCantainPath.find_first_of("?#");

				// path string without query or fragment
				if (pathEndDelimiter == std::string::npos)
				{
					// strip beginning slash of path string
					auto beginningSlashPosition = uriMayCantainPath.find_first_of('/');
					if(beginningSlashPosition == 0)
					{
						isRelativePath = false;
						uriMayCantainPath = uriMayCantainPath.substr(1);
					}
					else
					{
						isRelativePath = true;
					}
					

					for(;;)
					{
						auto pathElementDelimiter = uriMayCantainPath.find("/");
						if(pathElementDelimiter != std::string::npos)
						{
							path.emplace_back(uriMayCantainPath.begin(), uriMayCantainPath.begin() + pathElementDelimiter);
							uriMayCantainPath = uriMayCantainPath.substr(pathElementDelimiter+1);
						}
						else	// no "/" found
						{
							path.push_back(uriMayCantainPath);
							uriMayCantainPath.clear();
							break;
						}
					}
					restString = "";
					return true;
					
				}
				else	// find query or fragment
				{
					restString = uriMayCantainPath.substr(pathEndDelimiter);
					
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
			if(!host.empty() && uriMayCantainPath.empty())
			{
				isRelativePath = false;
				path.push_back("");
				return true;
			}
			
			// If host doesn't exist and path is empty,
			// The path is relative path
			isRelativePath = true;
			restString = uriMayCantainPath;
			return true;
		}

		bool parseQuery(std::string& uriString, std::string& restString)
		{
			auto queryBeginDelimiter = uriString.find('?');
			auto queryEndDelimiter = uriString.find('#');
			if (queryBeginDelimiter != std::string::npos)
			{
				// has query
				hasQueryBool = true;
				if (queryEndDelimiter != std::string::npos)
				{
					query = uriString.substr(queryBeginDelimiter+1, queryEndDelimiter-1);
					restString = uriString.substr(queryEndDelimiter);
					return true;
				}
				else
				{
					query = uriString.substr(queryBeginDelimiter+1);
					restString = "";
					return true;
				}
				
			}
			else
			{
				hasQueryBool = false;
				restString = uriString;
				return true;
			}
		}
		
		bool parseFragment(std::string& uriMayContainFragment, std::string& restString)
		{
			auto fragmentBeginDelimiter = uriMayContainFragment.find('#');
			if (fragmentBeginDelimiter != std::string::npos)
			{
				// has fragment
				hasFragmentBool = true;
				fragment = uriMayContainFragment.substr(fragmentBeginDelimiter + 1);
				restString = "";
				return true;
			}
			else
			{
				hasFragmentBool = false;
				restString = uriMayContainFragment;
				return true;
			}
			
		}
		
		// absolute path begins with a "/"
		bool isAbsolutePath()
		{
			return (!path.empty() && !isRelativePath);
		}

		void removeDots()
		{
			std::vector< std::string > oldPath = std::move(path);
			path.clear();
			// Bitate: why use this?
			// Is this means that the resources directory
			bool atDirectoryLevel = false;

			for(const std::string segment : oldPath)
			{
				if(segment == ".")
				{
					atDirectoryLevel = true;
				}
				else if ( segment == "..")
				{
					if(!path.empty())
					{
						// remove last path element if we can navigate up a path level 
						if(!isAbsolutePath() || (path.size() > 1))
						{
							path.pop_back();
						}
					}
					atDirectoryLevel = true;
				}
				else
				{
					if( !atDirectoryLevel || ! segment.empty())
					{
						path.push_back(segment);
					}
					atDirectoryLevel = segment.empty();
				}
				
				
			}
			
			// if at the end of reconstructing the path,
			// we are in a directory level, 
			// add an empty segment to path vector end to notify
			if( atDirectoryLevel && (!path.empty() && !path.back().empty()))
			{
				path.push_back("");
			}
		}

	};
	
	Uri::~Uri() noexcept = default;
    
	Uri::Uri(const Uri& other) : impl_(new Impl)
    {
        *this = other;
    }
    
	Uri& Uri::operator=(const Uri& other) 
	{
        if (this != &other) 
		{
            *impl_ = *other.impl_;
        }
        return *this;
    }
    
	Uri::Uri(Uri&&) noexcept = default;

	Uri& Uri::operator=(Uri&&) noexcept = default;

    Uri::Uri() : impl_(new Impl)
    {
    }
	
	// Getters
	std::string Uri::getScheme()
	{
		return impl_->scheme;
	}

	std::string Uri::getUserInfo()
	{
		return impl_->userInfo;
	}
	
	std::string Uri::getHost()
	{
		return impl_->host;
	}

	std::vector< std::string > Uri::getPath()
	{
		return impl_->path;
	}

	std::string Uri::getPathString()
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

	int Uri::getPort()
	{
		return impl_->port;
	}

	std::string Uri::getQuery()
	{
		return impl_->query;
	}

	std::string Uri::getFragment()
	{
		return impl_->fragment;
	}

	// Setters
	bool Uri::setScheme(std::string& s)
	{
		if (impl_->scheme.empty())
		{
			impl_->scheme = s;
			return true;
		}
		return false;
	}

	bool Uri::setHost(std::string& host)
	{
		if (impl_->host.empty()) 
		{ 
			impl_->host = host; 
			return true;
		}
		return false;
	}

	bool Uri::setPort(int& p)
	{
		impl_->port = p;
		return true;
	}


	bool Uri::setQuery(std::string q)
	{
		impl_->query = q;
		if (impl_->query.empty()) { return false; }
		return true;
	}

	bool Uri::setFragment(std::string& s)
	{
		if (impl_->fragment.empty())
		{
			impl_->fragment = s;
			return true;
		}
		return false;
	}

	bool Uri::clearPort()
	{
		impl_->port = -1;
		return true;
	}

	bool Uri::clearQuery()
	{
		if (!impl_->query.empty())
		{
			impl_->query.clear();
			return true;
		}
		return false;
	}

	bool Uri::clearFragment()
	{
		if (!impl_->fragment.empty())
		{
			impl_->fragment.clear();
			return true;
		}
		return false;
	}

	bool Uri::clearScheme()
	{
		impl_->scheme.clear();
		return true;
	}

	bool Uri::hasPort()
	{
		return impl_->hasPortBool;
	}

	bool Uri::hasQuery()
	{
		return impl_->hasQueryBool;
	}

	bool Uri::hasFragment()
	{
		return impl_->hasFragmentBool;
	}


	bool Uri::parseFromString(const std::string& uriString)
	{
		PercentEncoding percentEncoding;
		std::string decodedString = percentEncoding.Decode(uriString);

		std::string uriStringWithoutScheme;

		if(!impl_->parseScheme(decodedString, uriStringWithoutScheme))
		{
			return false;
		}

		std::string uriStringWithoutAuthority;
		if(!impl_->parseAuthority(uriStringWithoutScheme, uriStringWithoutAuthority))
		{
			return false;
		}
		
		std::string uriStringWithoutPath;
		if(!impl_->parsePath(uriStringWithoutAuthority, uriStringWithoutPath))
		{
			return false;
		}
		
		std::string uriStringWithoutQuery;
		if(!impl_->parseQuery(uriStringWithoutPath, uriStringWithoutQuery))
		{
			return false;
		}
		
		std::string uriStringWithoutFragment;
		if(!impl_->parseFragment(uriStringWithoutQuery, uriStringWithoutFragment))
		{
			return false;
		}

		return true;
	}

	// Bitate: the conditions of relative reference
	bool Uri::isRelativeReference()
	{
		return impl_->scheme.empty();
	}

	bool Uri::hasRelativePath()
	{
		return !impl_->isAbsolutePath();
	}

}
