#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
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
	 *		client_socket is the established socket connection.
	 *
	 * @return 
	 *		true/false.
	 */
	bool listen_at(const std::string& host, const int port);

	/**
	 * @brief
	 * 		Send generated response to client.
	 * @return 
	 *		true/false.
	 */
	bool send_response();

	/**
	 * @brief
	 * 		Receive request from client.
	 * @return 
	 *		true/false.
	 */
	bool receive_request();
	
	/**
	 * @brief
	 * 		Parse raw request.
	 * @return
	 * 		true/false.
	 */
	bool parse_request();

	/**
	 * @brief
	 * 		Generate response message.
	 * @return 
	 * 		true/false.
	 */
	bool generate_response();

	/**
	 * @brief
	 * 		Get raw request string.
	 * @return 
	 * 		Raw request string.
	 */
	std::string get_raw_request();

	/**
	 * @brief
	 * 		Get client socket file descriptor.
	 * @return 
	 * 		Success: Client socket file descriptor
	 * 		Error: -1
	 */
	int get_client_fd();

	/**
	 * @brief
	 * 		Get server socket file descriptor.
	 * @return 
	 * 		Success: Server socket file descriptor
	 * 		Error: -1
	 */
	int get_server_fd();
	
private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};

