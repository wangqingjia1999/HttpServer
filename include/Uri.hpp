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
	public:
		~Uri() noexcept;
        Uri();

		Uri(const Uri& other);
		Uri& operator=(const Uri& other);

        Uri(Uri&&) noexcept;
        Uri& operator=(Uri&&) noexcept;
	public:
		/**
		 * @brief  Get scheme of URI.
		 * @return  Scheme string.
		 */
		std::string get_scheme();

		/**
		 * @brief  Get Host of URI.
		 * @return  Host string.
		 */
		std::string get_host();

		/**
		 * @brief  Get path vector of URI.
		 * @return  A vector of path elements
		 */
		std::vector< std::string > get_path();

		/**
		 * @brief  Get path string of URI.
		 * @return  A path string.
		 */
		std::string get_path_string();

		/**
		 * @brief  Get port number of URI.
		 * @return  A int number represents port.
		 */
		int get_port();

		/**
		 * @brief  Get query string of URI.
		 * @return  A query string.
		 */
		std::string get_query();

		/**
		 * @brief  Get fragment string of URI.
		 * @return  A fragment string.
		 */
		std::string get_fragment();

		/**
		 * @brief  Get user info of URI.
		 * @return  A user info string.
		 */
		std::string get_user_info();

		
		/**
		 * @brief  Set scheme.
		 * @return  True if successfully set.
		 */
		bool set_scheme(std::string& scheme);

		/**
		 * @brief  Set host.
		 * @param  host  Host string.
		 * @return  True if successfully set.
		 */
		bool set_host(std::string& host);

		/**
		 * @brief  Set port.
		 * @param  port  Port int.
		 * @return  True if successfully set.
		 */
		bool set_port(int& port);

		/**
		 * @brief  Set query.
		 * @param  query  Query string.
		 * @return  True if successfully set.
		 */
		bool set_query(std::string query);

		/**
		 * @brief  Set fragment.
		 * @param  fragment  Fragment string.
		 * @return  True if successfully set.
		 */
		bool set_fragment(std::string& fragment);

		/**
		 * @brief  Set user info.
		 * @param  user_info  User info string.
		 * @return  True if successfully set.
		 */
		bool set_user_info(std::string& user_info);

		
		/**
		 * @brief  Clear port.
		 * @return  True if successfully clear.
		 */
		bool clear_port();

		/**
		 * @brief  Clear query.
		 * @return  True if successfully clear.
		 */
		bool clear_query();

		/**
		 * @brief  Clear fragment.
		 * @return  True if successfully clear..
		 */
		bool clear_fragment();

		/**
		 * @brief  Clear scheme.
		 * @return  True if successfully clear.
		 */
		bool clear_scheme();

		/**
		 * @brief  Clear path.
		 * @return  True if successfully clear.
		 */
		bool clear_path();

		/**
		 * @brief  Parse an URI string.
		 * @param  uri_string  URI string to be parsed.
		 * @return  True if successfully parse.
		 */
		bool parse_from_string(const std::string& uri_string);
		
		/**
		 * @brief  Has port?
		 * @return  True if it has port.
		 */
		bool has_port();

		/**
		 * @brief  Has fragment?
		 * @return  True if it has fragment.
		 */
		bool has_fragment();

		/**
		 * @brief  Has query?
		 * @return  True if it has query.
		 */
		bool has_query();

		/**
		 * @brief  Is relative reference?
		 * @return  True if it is relative reference.
		 */  
		bool is_relative_reference();

		/**
		 * @brief  Has relative path?
		 * @return  True if it has relativ path.
		 */
		bool has_relative_path();

	private:
		/**
         * @brief  This is the type of structure that contains the private
         * properties of the instance.  It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
		struct Impl;

		/**
         * @brief  This contains the private properties of the instance.
         */
		std::unique_ptr< Impl > impl_;
	};
}


