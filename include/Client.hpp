#pragma once

#include "Request.hpp"
#include "Response.hpp"

class Client
{
	// Life-cycle management
public:
	~Client();
	Client();

	Client(const Client&);
	Client& operator=(const Client&) ;
	
	Client(Client&&) noexcept;
	Client& operator=(Client&&) noexcept;
public:
	/**
	 * @brief  Connect to target server.
	 * @return  True if successfully connect to server.
	 */
	bool connect_to();
	
	/**
	 * @brief  Send request to server.
	 * @return  True if successfully send request to server.
	 */
	bool send_request();

	/** 
	 * @brief  Parse received response from Server.
	 * @return  True if successfully parse the response.
	 */
	bool parse_response();

	/**
	 * @brief  Generate request message.
	 * @return  True if successfully generate message.
	 */
	bool generate_request();

	/**
	 * @brief  Parse Uri string.
	 * @param  uri  Uri string.
	 * @return  True if successfully parse Uri.
	 */
	bool parse_uri(const std::string uri);

	/**
	 * @brief  Get generated request string.
	 * @return  Generated request string.
	 */
	std::string get_generated_request();

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};