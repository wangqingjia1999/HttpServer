#include "Worker.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>

namespace
{
    int read_message(const int master_socket)
    {
        struct msghdr message = { 0 };

        char data_buffer[2];

        struct iovec io_vector = {
            .iov_base = data_buffer,
            .iov_len = sizeof(data_buffer)
        };

        message.msg_iov = &io_vector;
        message.msg_iovlen = 1;

        char control_message_buffer[sizeof(int)];
        message.msg_control = &control_message_buffer;
        message.msg_controllen = sizeof(control_message_buffer);

        if(recvmsg(master_socket, &message, 0) == -1)
        {
            Logger::error("recvmsg() error: " + std::string{ strerror(errno) });
            return -1;
        }

        struct cmsghdr* control_message = CMSG_FIRSTHDR(&message);
        int accept_event_fd = static_cast<int>( *(CMSG_DATA(control_message)) );
        delete control_message;
        control_message = nullptr;
        return accept_event_fd;
    }
}

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