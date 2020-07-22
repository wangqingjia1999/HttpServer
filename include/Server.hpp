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
	 * @param[in] 
	 *		port is the port number. e.g. 1080.
	 * @param[out] 
	 *		client_socket is the established socket connection.
	 * @return
	 * 		true if succeeds in listening given host:port.
	 * 		false if any error happens.
	 */
	bool listen_at(const std::string& host, const int port);

	/**
	 * @brief
	 * 		Send generated response to client.
	 * @return 
	 *		true/false.
	 */
	bool send_response(const int clietn_socket_fd);

	/**
	 * @brief
	 * 		Receive request from client.
	 * @param[in]
	 * 		client_socket_fd is the accepted socket fd.
	 * @return 
	 *		true/false.
	 */
	bool receive_request(const int client_socket_fd);
	
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
	 * 		Get raw response string.
	 * @return 
	 * 		Raw response string.
	 */
	std::string get_raw_response();

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
	
	/**
	 * @brief
	 * 		Set raw request string.
	 * @return 
	 * 		bool.
	 */
	void set_raw_request(const std::string& raw_request);


private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};

