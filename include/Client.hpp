#pragma once

#include "Request.hpp"
#include "Response.hpp"



class Client
{
public:
	~Client();
	Client();
	Client(const Client&) noexcept;
	Client(Client&&) noexcept;
	Client& operator=(const Client&) noexcept;
	Client& operator=(Client&&) noexcept;
public:
	/**
	 * @brief
	 * 		Connect to target server.
	 * @param[in] 
	 * 		Host string.
	 * @param[in]
	 * 		Port string.
	 * @return
	 * 		true/false.
	 */
	bool connect_to();
	
	/**
	 * @brief
	 * 		Send request to server.
	 * @return 
	 * 		true/false.
	 */
	bool send_request();

	/** 
	 * @brief
	 * 		Parse received response from Server.
	 * @return
	 *		true/false.
	 */
	bool parse_response();

	/**
	 * @brief
	 * 		Generate request message.
	 * @return 
	 * 		true/false
	 */
	bool generate_request();

	/**
	 * @brief
	 * 		Parse uri string.
	 * @param[in]
	 * 		Uri string.
	 * @return
	 * 		true/false
	 */
	bool parse_uri(const std::string uri);

	/**
	 * @brief
	 * 		Get generated request string.
	 * @return
	 * 		Generated Request String.
	 */
	std::string get_generated_request();

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};


