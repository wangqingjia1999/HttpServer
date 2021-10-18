#pragma once

#include "Cache.hpp"
#include "Connection.hpp"
#include "IResourceHandler.hpp"
#include "ServerConfiguration.hpp"
#include "WorkerSocket.hpp"

#include <memory>

#include <semaphore.h>

class Worker
{
public:
	Worker() = delete;
	~Worker() noexcept;

	Worker(const Worker& other);
	Worker& operator=(const Worker& other);

	Worker(Worker&& other);
	Worker& operator=(Worker&& other);

public:
	explicit Worker(const int worker_socket);

public:
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
	 * Handle/Process given request to fetch/get either static or dynamic
	 * resources.
	 *
	 * @param[in] raw_request_string
	 * 		Raw request string.
	 *
	 * @note This is currently the core function to handler either static or
	 * dynamic request.
	 */
	void request_core_handler(const std::string& raw_request_string);

	void event_loop();

private:
	int m_epfd;

	int m_worker_socket;

	std::unique_ptr<WorkerSocket> m_worker_socket_handler;
	std::shared_ptr<HTTP::Connection> m_connection;
	std::unique_ptr<WorkerSocket> m_server_socket;
	std::unique_ptr<IResourceHandler> m_resource_handler;
	std::map<std::string, std::string> post_data_map;
};