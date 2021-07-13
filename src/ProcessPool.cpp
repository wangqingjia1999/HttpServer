#include "ProcessPool.hpp"
#include "Worker.hpp"
#include "ServerSocket.hpp"

#include <vector>
#include <exception>
#include <iostream>

#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <semaphore.h>

namespace 
{
    bool send_message(const int worker_socket, const int fd)
    {
        char data_buffer[] = " ";
        struct iovec io_vector = {
            .iov_base = &data_buffer,
            .iov_len = sizeof(data_buffer)
        };

        union
        {
            char control_message_buffer[CMSG_SPACE(sizeof(fd))];
            struct cmsghdr control_message;
        } control_message;

        struct msghdr message = {
            .msg_iov = &io_vector,
            .msg_iovlen = 1,
            .msg_control = control_message.control_message_buffer,
            .msg_controllen = sizeof(control_message.control_message_buffer)
        };

        struct cmsghdr* first_control_message = CMSG_FIRSTHDR(&message);
        first_control_message->cmsg_len = CMSG_LEN(sizeof(fd));
        first_control_message->cmsg_level = SOL_SOCKET;
        first_control_message->cmsg_type = SCM_RIGHTS;

        memcpy(CMSG_DATA(first_control_message), &fd, sizeof(fd));

        if(sendmsg(worker_socket, &message, 0) != 0)
        {
            Logger::error("sendmsg() in master error: " + std::string{ strerror(errno) });
            return false;
        }
        
        return true;
    }
}

ProcessPool::ProcessPool()
    : m_cpu_cores{ get_nprocs() },
      m_ready_worker_socket_fd{ -1 },
      m_ready_worker_socket_shared_name{ "ready_worker_socket_shared" },
      m_accept_mutex_name{ "worker_accept_mutex" },
      m_worker_mutex_name{ "worker_mutex" }
{   
    initialize_process_pool();

    for(int i = 0; i < m_cpu_cores; ++i)
    {
        /**
         * fds[0]: master writing socket
         * fds[1]: worker reading socket
         */
        int fds[2];
        
        if(socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, fds) == -1)
        {
            Logger::error("socketpair() error: " + std::string{ strerror(errno) });
            throw std::runtime_error("socketpair() error");
        }
        
        pid_t child;
        switch(child = fork())
        {
            case -1:
            {
                Logger::error("fork() error: " + std::string{ strerror(errno) });
                throw std::runtime_error("fork() error");
            }

            case 0:
            {
                Worker worker(fds, m_accept_mutex_name, m_worker_mutex_name, "ready_worker_socket_shared");

                worker.loop();
                
                exit(EXIT_SUCCESS);
                break;
            }

            default:
            {
                m_worker_channels.push_back( Channel{ child, fds[0] } );
                break;
            }
        }
    }

    // master process code starts here
    ServerSocket server_socket("0.0.0.0", 80);

    for(;;)
    {
        switch(server_socket.listen_at())
        {
            case Server_Socket_State::NEW_SOCKET:
            {
                // wake up one worker to accept it
                sem_post(m_accept_mutex);
                
                if(*m_ready_worker_socket == -1)
                    // no worker

                break;
            }

            case Server_Socket_State::ERROR_SOCKET:
            {
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

ProcessPool::~ProcessPool()
{
    close(m_ready_worker_socket_fd);
    sem_close(m_worker_mutex);
    sem_close(m_accept_mutex);

    delete m_accept_mutex;
    m_accept_mutex = nullptr;

    delete m_worker_mutex;
    m_worker_mutex = nullptr;
}

void ProcessPool::initialize_process_pool()
{
    m_ready_worker_socket_fd = shm_open(m_ready_worker_socket_shared_name.c_str(), O_CREAT | O_RDWR, 0644);
    if(m_ready_worker_socket_fd == -1)
    {
        Logger::error("shm_open() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("shm_open() error");
    }

    int ftruncate_result = ftruncate(m_ready_worker_socket_fd, sizeof(int));
    if(ftruncate_result == -1)
    {
        Logger::error("ftruncate() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("ftruncate() error");
    }

    void* ready_worker_socket = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, m_ready_worker_socket_fd, 0);
    if(ready_worker_socket == SEM_FAILED)
    {
        Logger::error("mmap() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("mmap() error");
    }

    m_ready_worker_socket = static_cast<int*>(ready_worker_socket);

    m_worker_mutex = sem_open(m_worker_mutex_name.c_str(), O_CREAT, 0644, 1);
    if(m_worker_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() error");
    }

    m_accept_mutex = sem_open(m_accept_mutex_name.c_str(), O_CREAT, 0644, 0);
    if(m_accept_mutex == SEM_FAILED)
    {
        Logger::error("sem_open() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("sem_open() error");
    }
}

int ProcessPool::get_cpu_cores() const
{
    return m_cpu_cores;
}

void ProcessPool::close_pool()
{
    
    
}