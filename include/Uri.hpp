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
	 * Get scheme of Uri.
	 * 
	 * @return  
	 * 		Scheme string.
	 */
	std::string get_scheme();

	/**
	 * Get Host of Uri.
	 * 
	 * @return  
	 * 		Host string.
	 */
	std::string get_host();

	/**
	 * Get path vector of Uri.
	 * 	
	 * @return  
	 * 		A vector of path elements
	 */
	std::vector< std::string > get_path();

	/**
	 * Get path string of Uri.
	 * 
	 * @return  
	 * 		A path string.
	 */
	std::string get_path_string();

	/**
	 * Get port number of Uri.
	 * 
	 * @return  
	 * 		A int number represents port.
	 */
	int get_port();

	/**
	 * Get query string of Uri.
	 * 
	 * @return  
	 * 		A query string.
	 */
	std::string get_query();

	/**
	 * Get fragment string of Uri.
	 * 
	 * @return  
	 * 		A fragment string.
	 */
	std::string get_fragment();

	/**
	 * Get user info of Uri.
	 * 
	 * @return  
	 * 		A user info string.
	 */
	std::string get_user_info();

	/**
	 * Parse an Uri string.
	 * 
	 * @param[in] uri  
	 * 		Uri string to be parsed.
	 * 
	 * @return  
	 * 		True if successfully parse.
	 */
	bool parse_from_string(const std::string& uri);
	
	bool has_port();
	bool has_fragment();
	bool has_query();

	/**
	 * Is relative reference?
	 * 
	 * @return  
	 * 		True if it is relative reference.
	 */  
	bool is_relative_reference();

	/**
	 * Has relative path?
	 * 	
	 * @return  
	 * 		True if it has relativ path.
	 */
	bool has_relative_path();

private:
	/**
	 * Extract the scheme from uri if any,
	 * return the remaining string.
	 * 
	 * @param[in] uri 
	 * 		Uri string that may contain scheme.
	 * 
	 * @param[out] remains 
	 * 		Remaining string after parsing/extracting.
	 * 
	 * @return
	 * 		True if succeeds.
	 */
	bool parse_scheme(const std::string& uri, std::string & remains);

	/**
	 * Parse user info from Uri string.
	 * 
	 * @param[in] authority 
	 * 		Authority string.
	 * 
	 * @param[out] remains
	 * 		Remaining authority string with the user info substring being deleted.
	 * 
	 * @return 
	 * 		True if succeeds.
	 */
	bool parse_user_info(std::string& authority, std::string& remains);
	
	/**
	 * Parse host of an URI if any.
	 * 
	 * @param[in] authority
	 * 		Authority of an URI.
	 * 
	 * @param[out] remains
	 * 		Remains after weeding out the host.
	 * 
	 * @return 
	 * 		True if succeeds.
	 */
	bool parse_host(std::string& authority, std::string& remains);

	/**
	 * Parse port of an URI if any.
	 * 
	 * @param[in] authority
	 * 		Authority of an URI.
	 * 
	 * @param[out] remains
	 * 		Remains after weeding out the port.
	 *
	 * @return 
	 * 		True if succeeds.
	 */
	bool parse_port(std::string& authority);

	/**
	 * Parse authority of an URI if any.
	 * 
	 * @param[in] uri
	 * 		Given URI.
	 * 
	 * @param[out] remains
	 * 		Remains after weeding out the authority.
	 * 
	 * @return 
	 * 		True if succeeds.
	 */
	bool parse_authority(std::string& uri, std::string& remains);	

	/**
	 * Parse path of an URI if any.
	 * 
	 * @param[in] uri
	 * 		Given URI.		
	 * 
	 * @param[out] remains
	 * 		Remains after weeding out the path.
	 * 
	 * @return
	 * 		True if succeeds.
	 */
	bool parse_path(std::string& uri, std::string& remains);

	/**
	 * Parse query of an URI if any.
	 * 
	 * @param[in] uri
	 * 
	 * @param[out] remains 
	 * 		Remains after weeding out the query.
	 * 
	 * @return
	 * 		True if succeeds.
	 */
	bool parse_query(std::string& uri, std::string& remains);

	/**
	 * Parse fragment of an URI if any.
	 * 
	 * @param[in] uri
	 * 
	 * @param[out] remains 
	 * 		Remains after weeding out the fragment.
	 * 
	 * @return
	 * 		True if succeeds.
	 */
	bool parse_fragment(std::string& uri, std::string& remains);

	bool is_absolute_path();

	bool set_scheme(std::string& scheme);
	bool set_host(std::string& host);
	bool set_port(int port);
	bool set_query(std::string query);
	bool set_fragment(std::string& fragment);
	bool set_user_info(std::string& user_info);

	bool clear_port();
	bool clear_query();
	bool clear_fragment();
	bool clear_scheme();
	bool clear_path();
	
private:
	std::string m_scheme;

	std::string m_authority;

	std::string m_user_info;

	std::string m_host;

	bool m_has_port;
	uint16_t m_port;

	bool m_is_relative_path;
	std::vector < std::string > m_path;

	bool m_has_query;
	std::string m_query;

	bool m_has_fragment;
	std::string m_fragment;
};
