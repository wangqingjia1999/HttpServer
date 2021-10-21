#pragma once

#include <string>

enum class Server_Socket_State
{
	NEW_SOCKET,
	ERROR_SOCKET,
	UNKNOWN_SOCKET
};

class WorkerSocket
{
public:
	WorkerSocket() = default;
	~WorkerSocket() = default;

	WorkerSocket(const WorkerSocket&) = delete;
	WorkerSocket& operator=(const WorkerSocket&) = delete;

	WorkerSocket(WorkerSocket&&) = delete;
	WorkerSocket& operator=(WorkerSocket&&) = delete;

	bool write_to(int client_socket, const std::string& data_string);

	/**
	 * Read from socket. Currently, we use fixed size buffer,
	 * because the request messages are usually short.
	 */
	bool read_from(int client_socket);

	std::string get_receive_buffer_string();

private:
	int m_epfd = -1;
	int m_listening_fd = -1;
	int m_listening_port = -1;
	std::string m_listening_ip = {};

	char m_receive_buffer[8192] = {0}; // NOLINT

	Server_Socket_State m_server_socket_state =
	    Server_Socket_State::UNKNOWN_SOCKET;
};
