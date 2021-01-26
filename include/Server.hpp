#pragma once

#include "Logger.hpp"
#include "WebSocket.hpp"
#include "Connection.hpp"
#include "ThreadPool.hpp"
#include "ServerSocket.hpp"
#include "StatusHandler.hpp"
#include "ServerConfiguration.hpp"

#include <queue>
#include <mutex>
#include <string>
#include <memory>
#include <thread>
#include <iostream>
#include <condition_variable>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
	 * Create a socket to listen to clients.
	 * 
	 * @param  host  
	 * 		Host name. e.g. "localhost".
	 * 
	 * @param  port  
	 * 		Port number. e.g. 80.
	 */
	void listen_at(const std::string& host, const int port); 
	
	/**
	 * Parse raw request.
	 * 
	 * @param  raw_request_string  
	 * 		Raw request string receive from client.
	 * 
	 * @return  
	 * 		Ture if successfully parse request.
	 */
	bool parse_request(const std::string& raw_request_string);

	/**
	 * Handle POST request.
	 * 
	 * @return  
	 * 		True if successfully handle it.
	 */
	bool handle_post_request();

	/**
	 * Get raw request string.
	 * 
	 * @return  
	 * 		Raw request string.
	 */
	std::string get_raw_request();

	/**
	 * Get raw response string.
	 * 
	 * @return  
	 * 		Raw response string.
	 */
	std::string get_raw_response();
	
	/**
	 * Set raw request string.
	 * 
	 * @param[in] raw_request_string
	 * 		Raw request string.
	 */
	void set_raw_request(const std::string& raw_request_string);

	/**
	 * Handle/Process given request to fetch/get either static or dynamic resources.  
	 * 
	 * @param[in] raw_request_string
	 * 		Raw request string.
	 * 
	 * @note This is currently the core function to handler either static or dynamic request.
	 */
	void request_core_handler(const std::string& raw_request_string);

private:
	/**
     * Whether the request is a WebSocket openning handshake request.
     * 
	 * @param[in]  request  Request object that has been parsed successfully.
     * 
	 * @return  
	 * 		True if it is WebSocket openning handshake request.
     * 
     * @note: this is in experimental, I'm not ensure it's satety :)
     */
    bool is_websocket_opening_handshake(std::shared_ptr< Message::Request >& request);

private:
	std::shared_ptr< Logger > logger;
	std::shared_ptr< ThreadPool > thread_pool;
	std::shared_ptr< Connection > m_connection;
	std::unique_ptr< ServerSocket > server_socket;
	std::unique_ptr< ServerConfiguration > m_configuration;

	std::map< std::string, std::string > post_data_map;
};
