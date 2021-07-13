#pragma once

#include "Connection.hpp"
#include "Channel.hpp"

#include <queue>
#include <unordered_set>

#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

class Master
{
public:
    Master() = delete;
    ~Master();

    Master(const Master&) = delete;
    Master& operator=(const Master&) = delete;
    
    Master(Master&&) = delete;
    Master& operator=(Master&&) = delete;
public:
    Master(const std::string& ip, const int port);

public:
    void event_loop();
    int listen_at();
    int listen_at(const std::string& ip, const int port);

private:
    void initialize();
    int get_cpu_cores() const;
    void spawn_worker();

private:
    int m_cpu_cores;

    std::vector<Channel> m_worker_channels;

    std::string m_ready_worker_socket_shared_name;

    int m_ready_worker_socket_fd;
    int* m_ready_worker_socket;

    std::string m_accept_mutex_name;
    sem_t* m_accept_mutex;
    
    std::string m_worker_mutex_name;
    sem_t* m_worker_mutex;

    std::string m_listening_ip;
    int m_listening_port;
    int m_listening_socket;
    std::queue<int> m_accept_queue;

    int m_epfd;
    struct epoll_event* m_triggered_events;

    std::unordered_set<int> m_worker_sockets;
};
