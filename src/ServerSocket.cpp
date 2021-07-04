#include "ServerSocket.hpp"
#include "Logger.hpp"

#include <exception>

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

ServerSocket::ServerSocket()
    : epfd{ -1 },
      listen_fd{ -1 },
      readable_fd{ -1 },
      has_finished_initialization{ false },
      triggered_events{ new epoll_event() },
      server_socket_state{ Server_Socket_State::UNINITIALIZED }
{
    epfd = epoll_create(EPOLL_INTEREST_LIST_SIZE);
    if(epfd < 0)
    {
        std::runtime_error("can't create epoll file descriptor due to " + std::string{strerror(errno)});
    }
}

ServerSocket::~ServerSocket()
{
    if(close(listen_fd) < 0)
        Logger::warn("close() on listening file descriptor error due to: " + std::string(strerror(errno)));
    if(close(epfd) < 0)
        Logger::warn("close() on epoll file descriptor error due to: " + std::string(strerror(errno)));
}

void ServerSocket::initialize_server_socket(const std::string ip, const int port)
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        throw std::runtime_error("can't create server listening socket, because" + std::string(strerror(errno)));
    }

    struct sockaddr_in ipv4_address;
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    
    if(ip == "localhost" || ip == "0.0.0.0")
        ipv4_address.sin_addr.s_addr = INADDR_ANY;
    else
        ipv4_address.sin_addr.s_addr = inet_addr(ip.c_str());
    
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    int enable = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        Logger::warn("Cannot set socket " + ip + ":" + std::to_string(port) + " to be reusable.");

    int bind_result = bind(listen_fd, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
        throw std::runtime_error("can't bind to address: " + ip + ":" + std::to_string(port) + " because " + std::string(strerror(errno)));

    int listen_result = listen(listen_fd, MAXIMUM_LISTENING_PENDING_QUEUE);
    if(listen_result == -1)
    {
        throw std::runtime_error("server socket can't listen");
    }
    
    Logger::info("Server is listening at " + ip + ":" + std::to_string(port));

    epoll_event epoll_event_helper;
    epoll_event_helper.data.fd = listen_fd;
    epoll_event_helper.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &epoll_event_helper);
    
    has_finished_initialization = true;
}

Server_Socket_State ServerSocket::listen_at(const std::string ip, const int port)
{
    if(!has_finished_initialization)
        initialize_server_socket(ip, port);

    int number_of_triggered_events;
    for (;;)
    {
        switch(number_of_triggered_events = epoll_wait(epfd, triggered_events, TRIGGERED_EVENTS_MAX_SIZE, -1))
        {
            case -1:
            {
                Logger::error("epoll_ctl error, because " + std::string(strerror(errno)));
                return Server_Socket_State::ERROR;
            }
            
            default:
            {
                int i = 0;
                for(; i < number_of_triggered_events; ++i)
                {
                    int triggered_fd = triggered_events[i].data.fd;
                    uint32_t triggered_event = triggered_events[i].events;

                    // New client arrives
                    if((triggered_fd == listen_fd) && (triggered_event & EPOLLIN))
                    {
                        sockaddr_in client_socket;
                        socklen_t client_socket_length = sizeof(client_socket);

                        int client_fd = accept(listen_fd, (sockaddr*)&client_socket, &client_socket_length);
                        if(client_fd < 0)
                        {
                            Logger::warn("call to accept() failed.");
                            close(listen_fd);
                            return Server_Socket_State::ERROR;
                        }

                        Logger::debug(
                            "accept client " 
                            + std::string(inet_ntoa(client_socket.sin_addr)) 
                            + ":" 
                            + std::to_string(htons(client_socket.sin_port))
                        );

                        if(!set_socket_non_blocking(client_fd))
                        {
                            Logger::debug("can't set socket to non-blocking");
                            continue;
                        }
                        else
                        {
                            epoll_event epoll_event_helper;
                            epoll_event_helper.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
                            epoll_event_helper.data.fd = client_fd;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &epoll_event_helper);
                        }
                    }
                    
                    // Peer Error
                    if(triggered_event & EPOLLERR)
                    {
                        Logger::debug(
                            "peer socket error: "
                            + std::string(strerror(errno))
                        );
                        int epoll_ctl_result = epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_fd, nullptr);
                        if(epoll_ctl_result < 0)
                            Logger::debug("after epoll error triggered we encounter " + std::string(strerror(errno)));
                        
                        return Server_Socket_State::ERROR;
                    }
                
                    // Ready for reading
                    if((triggered_fd != listen_fd) && (triggered_event & EPOLLIN))
                    {
                        // It peer shudown
                        if(triggered_event & EPOLLRDHUP)
                        {
                            close(triggered_fd);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_fd, nullptr);
                            Logger::debug(
                                "fd " 
                                + std::to_string(triggered_fd) 
                                + " shutdown wrting."
                            );
                            continue;
                        }
                        readable_fd = triggered_fd;
                        return Server_Socket_State::READABLE;
                    }
                }
            }
            break;
        }   // end of swtich()
    }   // end of for(;;)
}

bool ServerSocket::write_to(const std::string& data_string)
{
    return write_to(readable_fd, data_string);
}

bool ServerSocket::write_to(const int peer_fd, const std::string& data_string)
{
    int send_result = send(peer_fd, data_string.c_str(), data_string.size(), 0);
    
    if(send_result < 0)
    {
        Logger::warn("call to send() failed.");
        return false;
    }
    else if(send_result == 0)
    {
        return false;
    }

    return true;
}   

std::string ServerSocket::read_from()
{
    return read_from(readable_fd);
}

std::string ServerSocket::read_from(const int peer_fd)
{
    char local_receive_buffer[MAXIMUM_BUFFER_SIZE] = { 0 };
    ssize_t receive_result = 0;
    
    std::string local_receive_buffer_string;
    while( (receive_result = recv(peer_fd, &local_receive_buffer, MAXIMUM_BUFFER_SIZE, 0)) > 0)
        local_receive_buffer_string += std::string(local_receive_buffer);

    if(receive_result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
        return local_receive_buffer_string;
    }
    
    if(receive_result == 0)
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, peer_fd, nullptr);
        close(peer_fd);
    }

    if(receive_result < 0)
    {
        Logger::debug("Peer socket has unexpected error in reading from.");
        epoll_ctl(epfd, EPOLL_CTL_DEL, peer_fd, nullptr);
        close(peer_fd);
    }
    
    Logger::debug("Receive: " + local_receive_buffer_string);

    return local_receive_buffer_string;
}

std::vector<uint8_t>* ServerSocket::get_receive_buffer()
{
    return &receive_buffer;
}

std::vector<uint8_t>* ServerSocket::get_send_buffer()
{
    return &send_buffer;
}

bool ServerSocket::set_socket_non_blocking(const int socket_fd)
{
    if(socket_fd < 0)
        return false;

    int file_status_flags = fcntl(socket_fd, F_GETFL, 0);
    if(file_status_flags < 0)
        return false;
    
    file_status_flags |= O_NONBLOCK;
    return (fcntl(socket_fd, F_SETFL, file_status_flags) == 0);
}