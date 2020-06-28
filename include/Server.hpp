#pragma once

#ifdef _WIN32
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#endif

#include <string>
#include <memory>

#include "Request.hpp"
#include "Response.hpp"

class Server
{
public:
	~Server();
	Server();
	Server(const Server&) noexcept;
	Server(Server&&) noexcept;
	Server& operator=(const Server&) noexcept;
	Server& operator=(Server&&) noexcept;

	/**
	 * Create a socket to listen to clients.
	 *
	 * @param[in] 
	 *		host is the host name. e.g. "localhost"
	 *
	 * @param[in] 
	 *		port is the port number. e.g. 1080.
	 *
	 * @param[out] 
	 *		clientSocket is the established socket connection.
	 *
	 * @return 
	 *		true/false.
	 */
	bool listenAt(const std::string& host, const int port);

	/**
	 * @brief
	 * 		Send generated response to client.
	 * @return 
	 *		true/false.
	 */
	bool sendResponse();

	/**
	 * @brief
	 * 		Receive request from client.
	 * @return 
	 *		true/false.
	 */
	bool receiveRequest();
	
	/**
	 * @brief
	 * 		Parse raw request.
	 * @return
	 * 		true/false.
	 */
	bool parseRequest();

	/**
	 * @brief
	 * 		Generate response message.
	 * @return 
	 * 		true/false.
	 */
	bool generateResponse();
private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};

