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
    void listen_at();
    void listen_at(const std::string& ip, const int port);

private:
    int get_cpu_cores() const;
    void spawn_worker(const size_t number_of_worker);

private:
    int m_cpu_cores;

    std::vector<Channel> m_worker_channels;
    bool m_is_monitor_worker;

    std::queue<int> pending_client_sockets;

    std::string m_listening_ip;
    int m_listening_port;
    int m_listening_socket;

    int m_epfd;
};
