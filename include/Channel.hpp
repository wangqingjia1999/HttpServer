#pragma once

#include <sys/types.h>

/**
 * @brief Socket pair/channel between master and worker process.
 */
class Channel
{
public:
	Channel() = delete;

public:
	Channel(const int master_socket, const int worker_socket, const pid_t pid);

public:
	int get_master_socket();
	int get_worker_socket();
	pid_t get_worker_pid();

private:
	int m_master_socket;
	int m_worker_socket;
	pid_t m_worker_pid;
};