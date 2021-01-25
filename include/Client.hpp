#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "ClientSocket.hpp"

class Client
{
public:
	~Client();
	Client();

	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;
	
	Client(Client&&) noexcept = delete;
	Client& operator=(Client&&) noexcept = delete;
public:
	/**
	 * Connect to target server.
	 * 
	 * @return  
	 * 		True if successfully connect to server.
	 */
	bool connect_to();
	
	/**
	 * Send request to server.
	 * 
	 * @return  
	 * 		True if successfully send request to server.
	 */
	bool send_request();

	/** 
	 * Parse received response from Server.
	 * 	
	 * @return 
	 * 		True if successfully parse the response.
	 */
	bool parse_response();

	/**
	 * Generate request message.
	 * 
	 * @return  
	 * 		True if successfully generate message.
	 */
	bool generate_request();

	/**
	 * Parse Uri string.
	 * 
	 * @param[in] uri  
	 * 		Uri string.
	 * 
	 * @return  
	 * 		True if successfully parse Uri.
	 */
	bool parse_uri(const std::string uri);

	/**
	 * Get generated request string.
	 * 
	 * @return  
	 * 		Generated request string.
	 */
	std::string get_generated_request();

private:
	std::shared_ptr< ClientSocket > m_client_socket;
};