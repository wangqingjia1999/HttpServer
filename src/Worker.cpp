#include "Worker.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>

Worker::Worker(
    const int domain_socket[2], 
    const std::string& accept_mutex_name,
    const std::string& worker_mutex_name,
    const std::string& ready_worker_socket_shared_name
)
    : m_worker_socket{ domain_socket[0] },
      m_master_socket{ domain_socket[1] },
      m_accept_mutex_name{ accept_mutex_name },
      m_worker_mutex_name{ worker_mutex_name },
      m_ready_worker_socket_shared_name{ ready_worker_socket_shared_name }
{
    m_accept_mutex = sem_open(accept_mutex_name.c_str(), 0);
    if(m_accept_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() error");
    }

    m_worker_mutex = sem_open(m_worker_mutex_name.c_str(), 0);
    if(m_worker_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() in worker error");
    }

    int shared_memory_fd = shm_open(m_ready_worker_socket_shared_name.c_str(), O_RDWR, 0644);
    if(shared_memory_fd == -1)
    {
        Logger::error("shm_open() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("shm_open() in worker error");
    }

    void* ready_worker_socket = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shared_memory_fd, 0);
    if(ready_worker_socket == MAP_FAILED)
    {
        Logger::error("mmap() in worker error: " + std::string{ strerror(errno) });
        throw std::runtime_error("mmap() in worker error");
    }
    
    m_ready_worker_socket_shared = static_cast<int*>(ready_worker_socket);
}

Worker::~Worker()
{
    sem_unlink(m_worker_mutex_name.c_str());
    sem_unlink(m_accept_mutex_name.c_str());
    
    delete m_accept_mutex;
    m_accept_mutex = nullptr;

    delete m_worker_mutex;
    m_worker_mutex = nullptr;
}

void Worker::loop()
{
    for(;;)
    {
        sem_wait(m_accept_mutex);
        sem_wait(m_worker_mutex);

        *m_ready_worker_socket_shared = m_worker_socket;

        int accept_event_fd = read_message(m_master_socket); 

        sem_post(m_worker_mutex);
    }
}