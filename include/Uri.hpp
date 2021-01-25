#pragma once

#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <functional>

#include "CharacterSet.hpp"
#include "PercentEncoding.hpp"

class Uri
{
public:
	~Uri() noexcept;
	Uri();

	Uri(const Uri& other) = default;
	Uri& operator=(const Uri& other) = default;

	Uri(Uri&&) noexcept = default;
	Uri& operator=(Uri&&) noexcept = default;

public:
	/**
	 * Equality comparison operator.
	 * 
	 * @param[in] other 
	 * 		The other Uri to which to compare this Uri.
	 * 
	 * @return 
	 * 		True if equal.
	 */
	bool operator==(const Uri& other) const;

	/**
	 * @brief  Get scheme of Uri.
	 * @return  Scheme string.
	 */
	std::string get_scheme();

	/**
	 * @brief  Get Host of Uri.
	 * @return  Host string.
	 */
	std::string get_host();

	/**
	 * @brief  Get path vector of Uri.
	 * @return  A vector of path elements
	 */
	std::vector< std::string > get_path();

	/**
	 * @brief  Get path string of Uri.
	 * @return  A path string.
	 */
	std::string get_path_string();

	/**
	 * @brief  Get port number of Uri.
	 * @return  A int number represents port.
	 */
	int get_port();

	/**
	 * @brief  Get query string of Uri.
	 * @return  A query string.
	 */
	std::string get_query();

	/**
	 * @brief  Get fragment string of Uri.
	 * @return  A fragment string.
	 */
	std::string get_fragment();

	/**
	 * @brief  Get user info of Uri.
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
	 * @brief  Parse an Uri string.
	 * @param  uri_string  Uri string to be parsed.
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
	 * Extract the scheme from uri_string if any,
	 * return the remaining string.
	 * 
	 * @param[in] uri_string 
	 * 		Uri string that may contain scheme.
	 * 
	 * @param[out] rest_string 
	 * 		Remaining string after parsing/extracting.
	 * 
	 * @return
	 * 		True if succeeds.
	 */
	bool parse_scheme(const std::string& uri_string, std::string & rest_string);

	/**
	 * Parse user info from Uri string.
	 * 
	 * @param[in] authority_string 
	 * 		Authority string.
	 * 
	 * @param[out] authority_string_without_user_info
	 * 		Remaining authority string with the user info substring being deleted.
	 * 
	 * @return 
	 * 		True if succeeds.
	 */
	bool parse_user_info(std::string& authority_string, std::string& authority_without_user_info);

	bool parse_host(std::string& authority_without_user_info, std::string& authority_without_host);

	bool parse_port(std::string& authority_string);

	bool parse_authority(std::string& uri_string, std::string& rest_string);	

	bool parse_path(std::string& uri_may_contain_path, std::string& rest_string);

	bool parse_query(std::string& uri_string, std::string& rest_string);

	bool parse_fragment(std::string& uri_may_contain_fragment, std::string& rest_string);

	bool is_absolute_path();

	void remove_dots();
	
private:
	std::string scheme;

	std::string authority;

	std::string user_info;

	std::string host;

	bool m_has_port;
	uint16_t port;

	bool is_relative_path;
	std::vector < std::string > path;

	bool has_query_bool;
	std::string query;

	bool has_fragment_bool;
	std::string fragment;

	CharacterSet characterset;
};