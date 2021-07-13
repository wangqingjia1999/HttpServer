#pragma once

#include "Connection.hpp"

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
    Worker(
        const int domain_socket[2], 
        const std::string& accept_mutex_name,
        const std::string& worker_mutex_name,
        const std::string& ready_worker_socket_shared_name
    );

public:
    void loop();

private:
    int m_worker_socket;
    int m_master_socket;

    std::string m_accept_mutex_name;
    sem_t* m_accept_mutex;

    std::string m_worker_mutex_name;
    sem_t* m_worker_mutex;
    
    std::string m_ready_worker_socket_shared_name;
    int* m_ready_worker_socket_shared;
};