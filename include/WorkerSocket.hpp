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
	WorkerSocket();
	~WorkerSocket();

	WorkerSocket(const WorkerSocket&) = delete;
	WorkerSocket& operator=(const WorkerSocket&) = delete;

	WorkerSocket(WorkerSocket&&) = delete;
	WorkerSocket& operator=(WorkerSocket&&) = delete;

public:
	bool write_to(const int client_socket, const std::string& data_string);

	/**
	 * Read from socket. Currently, we use fixed size buffer,
	 * because the request messages are usually short.
	 */
	bool read_from(const int client_socket);

	std::string get_receive_buffer_string();

private:
	int m_epfd;

	int m_listening_fd;
	int m_listening_port;
	std::string m_listening_ip;

	char m_receive_buffer[8192]; // NOLINT

	Server_Socket_State m_server_socket_state;
};
