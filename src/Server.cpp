#include "Server.hpp"
#include <sys/epoll.h>
#include <iostream>

struct Server::Impl
{
    int client_socket;
    int server_socket;

    static const int buffer_length = 1024;
    char received_buffer[buffer_length] = { 0 };

    std::shared_ptr< Message::Response> response = std::make_shared< Message::Response >();
    std::shared_ptr< Message::Request> request = std::make_shared< Message::Request >();
};

Server::~Server() = default;
Server::Server() : impl_(new Impl)
{
}

Server::Server(const Server& other) noexcept
{
    *this = other;
}

Server& Server::operator=(const Server& other) noexcept
{
    if(this!=&other)
    {
        *impl_=*other.impl_;
    }
    return *this;
}

Server::Server(Server&&) noexcept = default;
Server& Server::operator=(Server&&) noexcept = default;

bool Server::listen_at(const std::string& host, const int port)
{
    // IPv4 socket address
    struct sockaddr_in ipv4_address;
    
    // create socket
    impl_->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(impl_->server_socket == -1)
    {
        return false;
    }

    // set address info
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = inet_addr(host.c_str());
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    // bind address to socket
    int bind_result = bind(impl_->server_socket, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
    {
        return false;
    }

    // listen at given host and port
    // Note that the listen socket file descriptor remains unchanged
    int listen_result = listen(impl_->server_socket, 4096);
    if(listen_result == -1)
    {
        return false;
    }

    // create epoll fd
    int epfd = epoll_create(1024);
    if(epfd < 0)
    {
        return false;
    }

    // create epoll event
    struct epoll_event ev;
    ev.data.fd = impl_->server_socket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, impl_->server_socket, &ev);

    struct epoll_event epoll_result[1024];

    // server core loop
    for (;;)
    {
        int nums = epoll_wait(epfd, epoll_result, 1024, -1);
        switch (nums)
        {
        case 0:
            std::cout << "Time out" << std::endl;
            break;
        case -1:
            std::cout << "Epoll wait error" << std::endl;
            break;
        default:
            {
                int i;
                for(i=0; i < nums; ++i)
                {
                    int arrived_socket_fd = epoll_result[i].data.fd;
                    uint32_t arrived_socket_event = epoll_result[i].events;
                    
                    // If listen socket has new client in the listening queue.
                    if(arrived_socket_fd == impl_->server_socket && (arrived_socket_event & EPOLLIN))
                    {
                        struct sockaddr_in client_socket_address;
                        socklen_t client_socket_length = sizeof(client_socket_address);

                        // accept new client socket
                        impl_->client_socket = accept(
                            impl_->server_socket, 
                            (struct sockaddr*)&client_socket_address, 
                            &client_socket_length 
                        );
                        if(impl_->client_socket < 0)
                        {
                            std::cout << "Accept Error" << std::endl;
                            continue;
                        }

                        std::cout << "Accept Client from : " << inet_ntoa(client_socket_address.sin_addr) << ":" << htons(client_socket_address.sin_port) << std::endl;
                        
                        // and add new client socket into epoll events list
                        ev.data.fd = impl_->client_socket;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, impl_->client_socket, &ev);

                    }
                    else if (epoll_result[i].events & EPOLLIN) // receive request from client
                    {
                        int read_n_bytes = read(arrived_socket_fd, impl_->received_buffer, impl_->buffer_length);
                        if(read_n_bytes < 0)
                        {
                            std::cout << "Error: read from client" << std::endl;
                            continue;
                        }
                        
                        // ev.data.ptr = cumtom_data_type;
                        ev.data.fd = arrived_socket_fd;
                        ev.events = EPOLLOUT | EPOLLET;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, arrived_socket_fd, &ev);
                    }
                    else if (epoll_result[i].events & EPOLLOUT) // send response to client
                    {
                        
                        
                    }
                    else if (arrived_socket_fd != impl_->server_socket)
                    {


                    }
                }
                

            }
            break;
        }
    }
    

    return true;
}

bool Server::send_response()
{
    int sendResult = send(
        impl_->client_socket, 
        impl_->response->get_response_message().c_str(), 
        impl_->response->get_response_length(), 
        0
    );

    if(sendResult == -1)
    {
        return false;
    }
    return true;
}

bool Server::receive_request()
{
    int receive_result = recv(impl_->client_socket, impl_->received_buffer, impl_->buffer_length, 0);
    if(receive_result == 0 || receive_result == -1)
    {
        impl_->request->set_raw_request("");
        return false;
    }
    
    if(!impl_->request->set_raw_request(impl_->received_buffer))
    {
        return false;
    }

    memset(impl_->received_buffer, 0, impl_->buffer_length);

    return true;
}

bool Server::parse_request()
{
    return impl_->request->parse_raw_request();
}

bool Server::generate_response()
{
    return impl_->response->generate_response();
}

std::string Server::get_raw_request()
{
    return impl_->request->get_raw_request();
}

int Server::get_client_fd()
{
    return impl_->client_socket;
}

int Server::get_server_fd()
{
    return impl_->server_socket;
}