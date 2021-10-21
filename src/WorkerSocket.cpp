#include "WorkerSocket.hpp"
#include "Logger.hpp"

#include <exception>
#include <sys/socket.h>
#include <sys/types.h>

bool WorkerSocket::write_to(int client_socket, const std::string& data_string)
{
	int send_result =
	    send(client_socket, data_string.c_str(), data_string.size(), 0);

	if (send_result < 0)
	{
		Logger::error("worker send() error", errno);
		return false;
	}

	return true;
}

bool WorkerSocket::read_from(int client_socket)
{
	memset(&m_receive_buffer, 0, sizeof(m_receive_buffer));

	for (;;)
	{
		ssize_t receive_result = recv(client_socket, &m_receive_buffer,
		                              sizeof(m_receive_buffer), MSG_DONTWAIT);

		if (receive_result == -1)
		{
			// read error
			if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
			{
				Logger::error("worker recv() error", errno);
				close(client_socket);
				return false;
			}

			// read the whole buffer
			return true;
		}
	}

	return true;
}

std::string WorkerSocket::get_receive_buffer_string()
{
	return m_receive_buffer;
}