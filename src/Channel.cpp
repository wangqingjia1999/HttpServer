#include "Channel.hpp"


Channel::Channel(const pid_t pid, const int socket)
    : m_worker_socket{ socket },
      m_worker_pid{ pid }
{
}

void Channel::set_worker_pid(const pid_t pid)
{
    m_worker_pid = pid;
}

void Channel::set_worker_socket(const int socket)
{
    m_worker_socket = socket;
}

int Channel::get_worker_socket()
{
    return m_worker_socket;
}

pid_t Channel::get_worker_pid()
{
    return m_worker_pid;
}
