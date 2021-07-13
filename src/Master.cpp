#include "Master.hpp"
#include "Worker.hpp"
#include "ServerSocket.hpp"
#include "UnixDomainHelper.hpp"

#include <vector>
#include <exception>
#include <iostream>

#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>

namespace
{
    /**
     * Epoll triggered events max size
     */
    constexpr size_t TRIGGERED_EVENTS_MAX_SIZE = 1024;

    /**
     * Epoll interest/event list size
     */
    constexpr size_t EPOLL_INTEREST_LIST_SIZE = 1024;

    /**
     * Maximum socket listening buffer size in byte
     */
    constexpr size_t MAXIMUM_LISTENING_PENDING_QUEUE = 4096;

    /**
     * Maximum sending/receiving buffer size in byte
     */
    constexpr size_t MAXIMUM_BUFFER_SIZE = 8192;
}

Master::Master(const std::string& ip, const int port)
    : m_cpu_cores{ get_nprocs() },
      m_ready_worker_socket_fd{ -1 },
      m_ready_worker_socket_shared_name{ "ready_worker_socket_shared" },
      m_accept_mutex_name{ "worker_accept_mutex" },
      m_worker_mutex_name{ "worker_mutex" },
      m_listening_ip{ ip },
      m_listening_port{ port },
      m_listening_socket{ -1 },
      m_epfd{ -1 },
      m_triggered_events{ nullptr }
{
    initialize();
    spawn_worker();
    event_loop();
}

Master::~Master()
{
    close(m_ready_worker_socket_fd);
    sem_close(m_worker_mutex);
    sem_close(m_accept_mutex);

    delete m_accept_mutex;
    m_accept_mutex = nullptr;

    delete m_worker_mutex;
    m_worker_mutex = nullptr;
}

void Master::initialize()
{
    m_epfd = epoll_create(EPOLL_INTEREST_LIST_SIZE);
    if(m_epfd == -1)
    {
        Logger::error("master epoll_create() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("master epoll_create() error");
    }

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

int Master::get_cpu_cores() const
{
    return m_cpu_cores;
}

void Master::spawn_worker()
{
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
        
        pid_t child_pid;
        switch(child_pid = fork())
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
                m_worker_channels.push_back( Channel{ child_pid, fds[0] } );
                m_worker_sockets.insert(fds[0]);
                break;
            }
        }
    }
}

int Master::listen_at(const std::string& ip, const int port)
{
    m_listening_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if(m_listening_socket == -1)
    {
        Logger::error("master socket() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("master socket() error");
    }

    struct sockaddr_in socket_address = { 0 };
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = (ip == "0.0.0.0") || (ip == "localhost") ? INADDR_ANY : inet_addr(ip.c_str());
    socket_address.sin_port = htons(port);

    int enable = 1;
    if(setsockopt(m_listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
    {
        Logger::error("master bind() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("master bind() error");
    }

    if(bind(m_listening_socket, (struct sockaddr*)(&socket_address), sizeof(socket_address)))
    {
        Logger::error("master bind() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("master bind() error");
    }

    if(listen(m_listening_socket, MAXIMUM_LISTENING_PENDING_QUEUE) == -1)
    {
        Logger::error("master listen() error: " + std::string{ strerror(errno) });
        throw std::runtime_error("master listen() error");
    }

    epoll_event listening_event;
    listening_event.data.fd = m_listening_socket;
    listening_event.events = EPOLLIN | EPOLLET;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listening_socket, &listening_event);
}

int Master::listen_at()
{
    return listen_at(m_listening_ip, m_listening_port);
}

void Master::event_loop()
{
    int sum = 0;
    for(;;)
    {
        switch(sum = epoll_wait(m_epfd, m_triggered_events, EPOLL_INTEREST_LIST_SIZE, -1))
        {
            case -1:
            {
                Logger::error("master epoll_ctl() error: " + std::string(strerror(errno)));
                throw std::runtime_error("master epoll_ctl() error");
            }

            default:
            {
                for(int i = 0; i < sum; ++i)
                {
                    int triggered_fd = m_triggered_events[i].data.fd;
                    uint32_t triggered_event =  m_triggered_events[i].events;
                    if((triggered_fd == m_listening_socket) && (triggered_event & EPOLLIN))
                    {
                        m_accept_queue.push(triggered_fd);
                        sem_post(m_accept_mutex);
                    }
                    
                    // if one worker obtains the accept lock
                    if((triggered_fd & EPOLLIN) && (m_worker_sockets.count(triggered_fd) != 0))
                    {
                        if(!m_accept_queue.empty())
                        {
                            if(UnixDomainHelper::send_fd(triggered_fd, m_accept_queue.front()))
                                m_accept_queue.pop();
                        }
                        else
                        {
                            Logger::error("accept_queue is empty, no fd sent to worker");
                        }
                        
                        if(epoll_ctl(m_epfd, EPOLL_CTL_DEL, triggered_fd, nullptr) == -1)
                            Logger::error("epoll delete error: " + std::string{ strerror(errno) });
                    }

                    if(triggered_event & EPOLLERR)
                    {
                        Logger::error("triggered socket error: "+ std::string{ strerror(errno) });
                        if(epoll_ctl(m_epfd, EPOLL_CTL_DEL, triggered_fd, nullptr) == -1)
                            Logger::error("epoll delete error: " + std::string{ strerror(errno) });
                        
                        continue;
                    }
                }
            }
        }
    }
}
