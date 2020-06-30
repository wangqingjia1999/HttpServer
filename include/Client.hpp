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
	bool connectTo();
	
	/**
	 * @brief
	 * 		Send request to server.
	 * @return 
	 * 		true/false.
	 */
	bool sendRequest();

	/** 
	 * @brief
	 * 		Parse received response from Server.
	 * @return
	 *		true/false.
	 */
	bool parseResponse();

	/**
	 * @brief
	 * 		Generate request message.
	 * @return 
	 * 		true/false
	 */
	bool generateRequest();

	/**
	 * @brief
	 * 		Parse uri string.
	 * @param[in]
	 * 		Uri string.
	 * @return
	 * 		true/false
	 */
	bool parseUri(const std::string uri);

	/**
	 * @brief
	 * 		Get generated request string.
	 * @return
	 * 		Generated Request String.
	 */
	std::string getGeneratedRequest();

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};


