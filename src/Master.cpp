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

Master::Master(const std::string& ip, const int port)
    : m_cpu_cores{ get_nprocs() },
      m_listening_ip{ ip },
      m_listening_port{ port },
      m_listening_socket{ -1 },
      m_epfd{ -1 }
{
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
                try
                {
                    Worker worker(fds[1]);

                    worker.event_loop();
                } catch (const std::exception& e)
                {
                    Logger::error("master exists with exception: " + std::string{e.what()});
                    exit(EXIT_FAILURE);
                }
                break;
            }

            default:
            {
                m_worker_channels.push_back( Channel{ fds[0], fds[1], child_pid } );
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
        Logger::error("master epoll adds listening socket error", errno);
        throw std::runtime_error("master epoll adds listening socket error");
    }

    for(auto worker_channel : m_worker_channels)
    {
        epoll_event worker_writable_event;
        worker_writable_event.data.fd = worker_channel.get_master_socket();

        // Level-triggered mode for monitoring worker's readiness.
        worker_writable_event.events = EPOLLOUT;
        if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, worker_channel.get_master_socket(), &worker_writable_event) == -1)
        {
            Logger::error("master epoll adds monitor for worker's writability error", errno);
            throw std::runtime_error("master epoll adds monitor for worker's writability error");
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
                    int triggered_fd = triggered_events[i].data.fd;
                    uint32_t triggered_event =  triggered_events[i].events;
                    
                    // new connection
                    if((triggered_event & EPOLLIN) && (triggered_fd == m_listening_socket))
                    {
                        sockaddr_in accepted_socket;
                        socklen_t accepted_socket_length;
                        
                        int accepted_fd = 0;
                        
                        for(;;)
                        {
                            accepted_fd = accept(m_listening_socket, (sockaddr*)(&accepted_socket), &accepted_socket_length);
                            if(accepted_fd > 0)
                                pending_client_sockets.push(accepted_fd);
                            else
                                break;
                        }
                    }

                    // worker is ready for processing
                    if((triggered_event & EPOLLOUT) && (triggered_fd != m_listening_socket) && (!pending_client_sockets.empty()))
                    {
                        int pending_client_socket = pending_client_sockets.front();

                        UnixDomainHelper::send_fd(triggered_fd, pending_client_socket);

                        close(pending_client_socket);
                        
                        pending_client_sockets.pop();
                        
                        continue;
                    }

                    if(triggered_event & EPOLLERR)
                    {
                        Logger::error("master: close() triggered socket in error status", errno);
                        close(triggered_fd);
                        continue;
                    }
                }
            }
        }
    }
}
