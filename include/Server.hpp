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
	/**
     * @brief  Whether the request is a WebSocket openning handshake request.
     * @param  request  Request object that has been parsed successfully.
     * @return  True if it is WebSocket openning handshake request.
     * 
     * Note: this is in experimental, I'm not ensure it's satety :)
     */
    bool is_websocket_opening_handshake(std::shared_ptr< Message::Request >& request);

private:
	Server_Socket server_socket;

	Logger logger;

	int client_socket_fd;
    
	int server_socket_fd;

    // Receive buffer
    static const int receive_buffer_length = 1024;
    char receive_buffer[receive_buffer_length] = { 0 };

    // Send buffer
    static const size_t SEND_BUFFER_LENGTH = 1024;
    std::string send_buffer = "";

    // Response object for generating response
    std::shared_ptr< Message::Response> response = std::make_shared< Message::Response >();

    // Server side request object that is responsible for parseing coming request.
    std::shared_ptr< Message::Request> request = std::make_shared< Message::Request >();

    // Data obtained from POST request, I just store those name:value pairs into a map :).
    std::map< std::string, std::string > post_data_map;

    // Thread pool to avoid the unnecessary creation and deletion of multiple threads.
    std::shared_ptr< Thread_Pool > thread_pool = std::make_shared< Thread_Pool >();
};

#endif