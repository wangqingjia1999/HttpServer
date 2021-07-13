#pragma once

#include "Connection.hpp"
#include "Channel.hpp"

#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

class ProcessPool
{
public:
    ProcessPool();
    ~ProcessPool();

    ProcessPool(const ProcessPool&) = delete;
    ProcessPool& operator=(const ProcessPool&) = delete;
    
    ProcessPool(ProcessPool&&) = delete;
    ProcessPool& operator=(ProcessPool&&) = delete;

public:
    int get_cpu_cores() const;
    void close_pool();

private:
    void initialize_process_pool();

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
};
