#include "Channel.hpp"


Channel::Channel(
    const int master_socket,
    const int worker_socket,
    const pid_t pid
)
    : m_master_socket{ master_socket },
      m_worker_socket{ worker_socket },
      m_worker_pid{ pid }
{
}

int Channel::get_master_socket()
{
    return m_master_socket;
}

int Channel::get_worker_socket()
{
    return m_worker_socket;
}

pid_t Channel::get_worker_pid()
{
    return m_worker_pid;
}
