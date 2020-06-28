#pragma once
#include <memory>
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
	 * Parse received response from Server.
	 * 
	 * @param[in] 
	 *		rawRequest is received raw response message.
		*
		* @return
		*		A shared_ptr to Response object that has been parsed.
		*/
	std::shared_ptr< Message::Response > parseResponse(const std::string& rawResponse);

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};


