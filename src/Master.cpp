#include "Master.hpp"
#include "Worker.hpp"
#include "WorkerSocket.hpp"
#include "UnixDomainHelper.hpp"

#include <vector>
#include <exception>
#include <iostream>

#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <cstdio>


namespace
{
    /**
     * Epoll triggered events max size
     */
    constexpr size_t EPOLL_TRIGGERED_EVENTS_MAX_SIZE = 1024;

    /**
     * Epoll interest/event list size
     */
    constexpr size_t EPOLL_INTEREST_LIST_SIZE = 1024;

    /**
     * Maximum socket listening buffer size in byte
     */
    constexpr size_t MAXIMUM_LISTENING_PENDING_QUEUE = 4096;
}

namespace
{
    sem_t* reopen_semaphore(const std::string& semaphore_name, const int initial_value)
    {
        if(sem_unlink(semaphore_name.c_str()) == -1)
        {
            Logger::error("master sem_unlink() error", errno);
            throw std::runtime_error("master sem_unlink() error");
        }


        sem_t* semaphore = sem_open(semaphore_name.c_str(), O_CREAT | O_EXCL , S_IRWXG | S_IRWXU | S_IRWXO, initial_value);
        if(semaphore == SEM_FAILED)
        {
            Logger::error("rerun sem_open() error", errno);
            throw std::runtime_error("rerun sem_open() error");
        }
        return semaphore;
    }
}

Master::Master(const std::string& ip, const int port)
    : m_cpu_cores{ get_nprocs() },
      m_accept_mutex_name{ "/worker_accept_mutex" },
      m_worker_mutex_name{ "/worker_mutex" },
      m_listening_ip{ ip },
      m_listening_port{ port },
      m_listening_socket{ -1 },
      m_epfd{ -1 }
{
    initialize();
    spawn_worker(m_cpu_cores);
    listen_at();
    event_loop();
}

Master::~Master()
{
    close(m_epfd);

    for(auto& worker_channel : m_worker_channels)
    {
        kill(worker_channel.get_worker_pid(), SIGKILL);
    }

    wait(NULL);

    sem_close(m_worker_mutex);
    sem_close(m_accept_mutex);

    if(sem_unlink(m_worker_mutex_name.c_str()) < 0)
        Logger::error("sem_unlink() worker mutex error", errno);

    if(sem_unlink(m_accept_mutex_name.c_str()) < 0)
        Logger::error("sem_unlink() accept mutex error", errno);

    delete m_accept_mutex;
    m_accept_mutex = nullptr;

    delete m_worker_mutex;
    m_worker_mutex = nullptr;
}

void Master::initialize()
{
    m_worker_mutex = sem_open(m_worker_mutex_name.c_str(), O_CREAT | O_EXCL, S_IRWXG | S_IRWXU | S_IRWXO, 1);
    if(m_worker_mutex == SEM_FAILED)
    {
        if(errno == EEXIST)
        {
            m_worker_mutex = reopen_semaphore(m_worker_mutex_name.c_str(), 1);

            if(m_worker_mutex == SEM_FAILED)
            {
                Logger::error("sem_open() error", errno);
                throw std::runtime_error("sem_open() error");
            }
        }
        else
        {
            Logger::error("sem_open() error", errno);
            throw std::runtime_error("sem_open() error");
        }   
    }

    m_accept_mutex = sem_open(m_accept_mutex_name.c_str(), O_CREAT | O_EXCL, S_IRWXG | S_IRWXU | S_IRWXO, 0);
    if(m_accept_mutex == SEM_FAILED)
    {
        if(errno == EEXIST)
        {
            m_accept_mutex = reopen_semaphore(m_accept_mutex_name.c_str(), 0);

            if(m_accept_mutex == SEM_FAILED)
            {
                Logger::error("sem_open() error", errno);
                throw std::runtime_error("sem_open() error");
            }
        }
        else
        {
            Logger::error("sem_open() error", errno);
            throw std::runtime_error("sem_open() error");
        }   
    }
}

int Master::get_cpu_cores() const
{
    return m_cpu_cores;
}

void Master::spawn_worker(const size_t number_of_worker)
{
    for(int i = 0; i < number_of_worker; ++i)
    {
        /**
         * fds[0]: master side socket
         * fds[1]: worker side socket
         */
        int fds[2];
        
        if(socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, fds) == -1)
        {
            Logger::error("socketpair() error", errno);
            throw std::runtime_error("socketpair() error");
        }
        
        pid_t child_pid;
        switch(child_pid = fork())
        {
            case -1:
            {
                Logger::error("fork() worker process error", errno);
                throw std::runtime_error("fork() error");
            }

            case 0:
            {
                Worker worker(
                    fds[1], 
                    m_accept_mutex_name, 
                    m_worker_mutex_name
                );

                worker.event_loop();
                
                exit(EXIT_SUCCESS);
                break;
            }

            default:
            {
                m_worker_channels.push_back( Channel{ fds[0], fds[1], child_pid } );
                m_master_sockets.insert(fds[0]);
                break;
            }
        }
    }
}

void Master::listen_at(const std::string& ip, const int port)
{
    m_listening_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if(m_listening_socket == -1)
    {
        Logger::error("master socket() error", errno);
        throw std::runtime_error("master socket() error");
    }

    struct sockaddr_in socket_address = { 0 };
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = (ip == "0.0.0.0") || (ip == "localhost") ? INADDR_ANY : inet_addr(ip.c_str());
    socket_address.sin_port = htons(port);

    int enable = 1;
    if(setsockopt(m_listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
    {
        Logger::error("master bind() error", errno);
        throw std::runtime_error("master bind() error");
    }

    if(bind(m_listening_socket, (struct sockaddr*)(&socket_address), sizeof(socket_address)) == -1)
    {
        Logger::error("master bind() error", errno);
        throw std::runtime_error("master bind() error");
    }

    if(listen(m_listening_socket, MAXIMUM_LISTENING_PENDING_QUEUE) == -1)
    {
        Logger::error("master listen() error", errno);
        throw std::runtime_error("master listen() error");
    }

    Logger::info("master listens at: " + m_listening_ip + ":" + std::to_string(m_listening_port));

    m_epfd = epoll_create(EPOLL_INTEREST_LIST_SIZE);
    if(m_epfd == -1)
    {
        Logger::error("master epoll_create() error", errno);
        throw std::runtime_error("master epoll_create() error");
    }

    epoll_event listening_event;
    listening_event.data.fd = m_listening_socket;
    listening_event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listening_socket, &listening_event) == -1)
    {
        Logger::error("master epoll add error", errno);
        throw std::runtime_error("master epoll add error");
    }
    
    // monitor workers' requests
    if(m_master_sockets.empty())
    {
        Logger::error("master sockets set is empty. worker process is not created correctly");
        throw std::runtime_error("master sockets set is empty.");
    }
    else
    {
        for(const int master_socket : m_master_sockets)
        {
            epoll_event worker_event;
            worker_event.data.fd = master_socket;
            worker_event.events = EPOLLIN | EPOLLET;
            if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, master_socket, &worker_event) == -1)
            {
                Logger::error("master epoll add error", errno);
                throw std::runtime_error("master epoll add error");
            }
        }
    }
    
}

void Master::listen_at()
{
    listen_at(m_listening_ip, m_listening_port);
}

void Master::event_loop()
{
    int sum = 0;
    struct epoll_event triggered_events[EPOLL_TRIGGERED_EVENTS_MAX_SIZE] = { 0 };
    for(;;)
    {
        switch(sum = epoll_wait(m_epfd, triggered_events, EPOLL_INTEREST_LIST_SIZE, -1))
        {
            case -1:
            {
                Logger::error("master epoll_wait() error", errno);
                continue;
            }

            default:
            {
                for(int i = 0; i < sum; ++i)
                {
                    int accepted_fd = 0;
                    int triggered_fd = triggered_events[i].data.fd;
                    uint32_t triggered_event =  triggered_events[i].events;
                    
                    // new connection
                    if((triggered_event & EPOLLIN) && (triggered_fd == m_listening_socket))
                    {
                        sockaddr_in accepted_socket;
                        socklen_t accepted_socket_length;
                        if((accepted_fd = accept(m_listening_socket, (sockaddr*)(&accepted_socket), &accepted_socket_length)) == -1)
                        {
                            Logger::error("master accept() error", errno);
                            continue;
                        }
                                            
                        m_accept_queue.push(accepted_fd);
                        sem_post(m_accept_mutex);
                    }

                    // if one worker obtains the accept lock, pass one fd to the worker.
                    if((triggered_event & EPOLLIN) && (m_master_sockets.find(triggered_fd) != m_master_sockets.end()))
                    {
                        if(!m_accept_queue.empty())
                        {
                            if(!UnixDomainHelper::send_fd(triggered_fd, m_accept_queue.front()))
                                Logger::error("master sends fd error");
                            close(m_accept_queue.front());
                            m_accept_queue.pop();
                        }
                    }

                    if(triggered_event & EPOLLERR)
                    {
                        Logger::error("triggered socket event error", errno);
                        if(epoll_ctl(m_epfd, EPOLL_CTL_DEL, triggered_fd, nullptr) == -1)
                            Logger::error("epoll delete error");
                        
                        continue;
                    }
                }
            }
        }
    }
}
