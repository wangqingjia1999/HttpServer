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
private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};


