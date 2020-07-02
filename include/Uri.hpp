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
		std::string get_scheme();

		/**
		 * This function is used to get Host.
		 *
		 * @param[in] s
		 *
		 * @return
		 *		A string of host.
		 */
		std::string get_host();

		/**
		 * @brief
		 * 		Get path vector.
		 * @return
		 *		A vector of path elements
		 */
		std::vector< std::string > get_path();

		/**
		 * @brief
		 * 		Get path string.
		 * @return 
		 * 		A path string.
		 */
		std::string get_path_string();

		/**
		 * @brief
		 * 		Get port number.
		 * @return
		 *		A int number of port.
		 */
		int get_port();

		/**
		 * This function is used to get query.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A query string.
		 */
		std::string get_query();

		/**
		 * This function is used to get fragment.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A fragment string.
		 */
		std::string get_fragment();

		/**
		 * This function is used to get user info.
		 *
		 * @param[in] s
		 *
		 * return
		 *		A user info string.
		 */
		std::string get_user_info();

		
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
		bool set_scheme(std::string& s);

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
		bool set_host(std::string& h);

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
		bool set_port(int& p);

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
		bool set_query(std::string q);

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
		bool set_fragment(std::string& s);

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
		bool set_user_info(std::string& u);

		
		/**
		 * Clear port field.
		 *
		 * @return
		 *		An indication of whether or not the
		 *		port is cleared successfully.
		 */
		bool clear_port();

		/**
		 * Clear query field
		 *
		 * @return
		 *		An indication of whether or not the
		 *		query is cleared is returned.
		 */
		bool clear_query();

		/**
		 * Clear fragment field.
		 *
		 * @return
		 *		An indication of whether or not the
		 *		fragment is cleared is returned.
		 */
		bool clear_fragment();

		/**
		 * Clear scheme field.
		 *
		 * return
		 *		An indication of whether or not
		 *		the scheme is cleared successfully.
		 */
		bool clear_scheme();

		/**
		 * Clear path field.
		 *
		 * return
		 *		An indication of whether or not
		 *		the path is cleared successfully.
		 */
		bool clear_path();

		/**
		 * Main uri-string-parser to parse the raw uri string.
		 *
		 * @param[in]
		 *
		 * @return
		 *		An indication of whether or not
		 *		the raw uri string is parsed successfully.
		 */
		bool parse_from_string(const std::string& uri_string);
		
		bool has_port();
		bool has_fragment();
		bool has_query();

		bool is_relative_reference();

		bool has_relative_path();

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


