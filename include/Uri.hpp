#pragma once

#include <string>
#include <vector>
#include <regex>

#include "CharacterSet.hpp"
#include "PercentEncoding.hpp"

namespace Uri
{
	class Uri
	{
		// Lifecycle management
	public:
		~Uri() noexcept;
        Uri(const Uri& other);
        Uri(Uri&&) noexcept;
        Uri& operator=(const Uri& other);
        Uri& operator=(Uri&&) noexcept;
		
	public:
		// constructor
		Uri();

		/**
		 * This function is used to get scheme.
		 *
		 * @param[in] s
		 *
		 * @return
		 *		A string of scheme.
		 */
		std::string getScheme();

		/**
		 * This function is used to get Host.
		 *
		 * @param[in] s
		 *
		 * @return
		 *		A string of host.
		 */
		std::string getHost();

		/**
		 * @brief
		 * 		Get path vector.
		 * @return
		 *		A vector of path elements
		 */
		std::vector< std::string > getPath();

		/**
		 * @brief
		 * 		Get path string.
		 * @return 
		 * 		A path string.
		 */
		std::string getPathString();

		/**
		 * @brief
		 * 		Get port number.
		 * @return
		 *		A int number of port.
		 */
		int getPort();

		/**
		 * This function is used to get query.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A query string.
		 */
		std::string getQuery();

		/**
		 * This function is used to get fragment.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A fragment string.
		 */
		std::string getFragment();

		/**
		 * This function is used to get user info.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A user info string.
		 */
		std::string getUserInfo();

		
		/**
		 * This function is used to set scheme.
		 *
		 * @param[in] s
		 *		A scheme string.
		 *
		 * @return
		 *		An indication of whether or not the scheme is set
		 *		is returned.
		 */
		bool setScheme(std::string& s);

		/**
		 * This function is used to set scheme.
		 *
		 * @param[in] h
		 *		A host string.
		 *
		 * @return
		 *		An indication of whether or not the host is set
		 *		is returned.
		 */
		bool setHost(std::string& h);

		/**
		 * This function is used to set scheme.
		 *
		 * @param[in] p
		 *		A port int.
		 *
		 * @return
		 *		An indication of whether or not the port is set
		 *		is returned.
		 */
		bool setPort(int& p);

		/**
		 * This function is used to set scheme.
		 *
		 * @param[in] q
		 *		A query string.
		 *
		 * @return
		 *		An indication of whether or not the query is set
		 *		is returned.
		 */
		bool setQuery(std::string q);

		/**
		 * This function is used to set scheme.
		 *
		 * @param[in] s
		 *		A scheme string.
		 *
		 * @return
		 *		An indication of whether or not the fragment is set
		 *		is returned.
		 */
		bool setFragment(std::string& s);

		/**
		 * This function is used to set user info.
		 *
		 * @param[in] u
		 *		A user info string.
		 *
		 * @return
		 *		An indication of whether or not the user info
		 *		is set is returned.
		 */
		bool setUserInfo(std::string& u);

		
		/**
		 * Clear port field.
		 *
		 * @return
		 *		An indication of whether or not the
		 *		port is cleared successfully.
		 */
		bool clearPort();

		/**
		 * Clear query field
		 *
		 * @return
		 *		An indication of whether or not the
		 *		query is cleared is returned.
		 */
		bool clearQuery();

		/**
		 * Clear fragment field.
		 *
		 * @return
		 *		An indication of whether or not the
		 *		fragment is cleared is returned.
		 */
		bool clearFragment();

		/**
		 * Clear scheme field.
		 *
		 * return
		 *		An indication of whether or not
		 *		the scheme is cleared successfully.
		 */
		bool clearScheme();

		/**
		 * Clear path field.
		 *
		 * return
		 *		An indication of whether or not
		 *		the path is cleared successfully.
		 */
		bool clearPath();

		/**
		 * Main uri-string-parser to parse the raw uri string.
		 *
		 * @param[in]
		 *
		 * @return
		 *		An indication of whether or not
		 *		the raw uri string is parsed successfully.
		 */
		bool parseFromString(const std::string& uriString);
		
		bool hasPort();
		bool hasFragment();
		bool hasQuery();

		bool isRelativeReference();

		bool hasRelativePath();

	private:
		/**
         * This is the structure that contains the private properties.  
		 * It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}


