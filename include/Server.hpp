#ifndef SERVER_HPP
#define SERVER_HPP

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif
#include <string.h>
#include <string>
#include <memory>

#include "Request.hpp"
#include "Response.hpp"

class Server
{
public:
	~Server() noexcept;
	Server();

	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	
	Server(Server&&) = delete;
	Server& operator=(Server&&) = delete;

	/**
	 * @brief  Create a socket to listen to clients.
	 * @param  host  Host name. e.g. "localhost".
	 * @param  port  Port number. e.g. 80.
	 * @return  True if successfully lisen at specific host:port.
	 */
	bool listen_at(const std::string& host, const int port); 

	/**
	 * @brief  Send generated response to client.
	 * @param  client_socket_fd  File descriptor of client socket.
	 * @param  buffer  Data to be sent.
	 */
	void send_response(const int clietn_socket_fd, const std::string& buffer);

	/**
	 * @brief  Receive request from client.
	 * @param  client_socket_fd  File descriptor of accepted socket
	 */
	void receive_request(const int client_socket_fd);
	
	/**
	 * @brief  Parse raw request.
	 * @return  Ture if successfully parse request.
	 */
	bool parse_request();

	/**
	 * @brief  Handle POST request.
	 * @return  True if successfully handle it.
	 */
	bool handle_post_request();

	/**
	 * @brief  Get raw request string.
	 * @return  Raw request string.
	 */
	std::string get_raw_request();

	/**
	 * @brief  Get raw response string.
	 * @return  Raw response string.
	 */
	std::string get_raw_response();

	/**
	 * @brief  Get client socket file descriptor.
	 * @return  Client socket file descriptor if succeed,
	 * 			otherwise, return -1.
	 */
	int get_client_fd();

	/**
	 * @brief  Get server socket file descriptor.
	 * @return  Server socket file descriptor if succeed,
	 * 			otherwise, return -1.
	 */
	int get_server_fd();
	
	/**
	 * @brief  Set raw request string.
	 */
	void set_raw_request(const std::string& raw_request);

	/**
	 * @brief  Handle/Process given request to fetch/get either static or dynamic resources.  
	 * 
	 * This is currently the core function to handler either static or dynamic request.
	 */
	void request_core_handler();

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};

#endif