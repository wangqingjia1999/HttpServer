#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "WebSocket.hpp"
#include "Thread_Pool.hpp"
#include "Status_Handler.hpp"
#include "Server_Socket.hpp"

#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#elif _WIN32

#endif


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
	 */
	void listen_at(const std::string& host, const int port); 
	
	/**
	 * @brief  Parse raw request.
	 * @param  raw_request_string  Raw request string receive from client.
	 * @return  Ture if successfully parse request.
	 */
	bool parse_request(const std::string& raw_request_string);

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
	 * @brief  Set raw request string.
	 */
	void set_raw_request(const std::string& raw_request);

	/**
	 * @brief  Handle/Process given request to fetch/get either static or dynamic resources.  
	 * 
	 * This is currently the core function to handler either static or dynamic request.
	 */
	void request_core_handler(const std::string& raw_request_string);

private:
	/**
     * @brief  Whether the request is a WebSocket openning handshake request.
     * @param  request  Request object that has been parsed successfully.
     * @return  True if it is WebSocket openning handshake request.
     * 
     * Note: this is in experimental, I'm not ensure it's satety :)
     */
    bool is_websocket_opening_handshake(std::shared_ptr< Message::Request >& request);

private:
	std::shared_ptr< Logger > logger;
	std::shared_ptr< Thread_Pool > thread_pool;
	std::shared_ptr< Message::Request > request;
	std::shared_ptr< Message::Response > response;
	std::unique_ptr< Server_Socket > server_socket;

	std::map< std::string, std::string > post_data_map;
};

#endif